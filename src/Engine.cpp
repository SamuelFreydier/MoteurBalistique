#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
bool Engine::s_realisticAirResistance = false;
Vector3 Engine::s_wind = Vector3();
int Engine::s_colorShift = 0;
Referential Engine::s_referential = Referential();


/**
 * @brief Cr�e un moteur permettant de g�rer jusqu'� un maximum de collisions par frame.
 * Il est �galement possible de sp�cifier un nombre d'it�rations � suivre pour la r�solution des collisions.
 * Par d�faut => 2 fois le nombre de collisions d�tect�es
 * @param maxContacts 
 * @param iterations 
*/
Engine::Engine( const int& maxContacts, const int& iterations )
    : m_contactResolver( iterations ), m_maxContacts( maxContacts )
{
    m_contacts = new ParticleContact[ maxContacts ];

    m_spontaneousCollisionGenerator = new ParticleSpontaneousCollision();
    m_spontaneousCollisionGenerator->m_restitution = 1.f;

    m_calculateIterations = ( iterations == 0 );
}

Engine::~Engine()
{
    while( !m_particles.empty() )
    {
        delete m_particles.back();
        m_particles.pop_back();
    }

    delete[] m_contacts;
    delete m_spontaneousCollisionGenerator;
}

/**
 * @brief R�cup�ration de l'instance du Singleton
 * @return 
*/
Engine* Engine::getInstance( const int& maxContacts, const int& iterations )
{
    if( s_engine == nullptr )
    {
        s_engine = new Engine( maxContacts, iterations );
    }

    return s_engine;
}


/**
 * @brief Tire une particule � une certaine position, avec un certain angle et une certaine pulsion
 * @param initialPos 
 * @param initialAngle 
 * @param initialSpeed 
 * @param mass 
 * @param radius 
 * @param color 
 * @param isFireball 
*/
void Engine::shootParticle(const Vector3& initialPos, const Vector3& initialVelocity , const float& mass, const float& radius, const Vector3& color, bool isFireball, bool m_showParticleInfos)
{
    // Id�alement il faudrait plut�t utiliser un design pattern comme une Factory si on pr�voit d'instancier plein de particules diff�rentes, �a serait plus extensible et facile � maintenir sur le long terme
    // Pour la phase 1, �a marche avec juste la boule de feu mais �a deviendra bien plus pertinent au fil du temps
    Particle* newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = new Fireball( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos, s_colorShift );
    }
    else
    {
        newParticle = new Particle( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos);
    }

    m_particles.push_back( newParticle );
}

/**
 * @brief Appelle chaque g�n�rateur de collisions pour signaler leurs collisions
 * @return Nombre de collisions g�n�r�es
*/
int Engine::generateContacts()
{
    if( m_contacts == nullptr )
    {
        return 0;
    }

    int limit = m_maxContacts;
    ParticleContact* nextContact = m_contacts;

    // On it�re dans tous les g�n�rateurs de collisions
    for( ContactGenerators::iterator gen = m_contactGenerators.begin(); gen != m_contactGenerators.end(); gen++ )
    {
        int used = ( *gen )->addContact( nextContact, limit );
        limit -= used;
        nextContact += used;

        // Plus de collision possible.
        if( limit <= 0 )
        {
            break;
        }
    }

    // On it�re pour v�rifier les collisions entre toutes les particules
    for( int firstParticleIdx = 0 ; firstParticleIdx < m_particles.size() && limit > 0; firstParticleIdx++  )
    {
        Particle* firstParticle = m_particles[ firstParticleIdx ];
        for( int secondParticleIdx = firstParticleIdx + 1; secondParticleIdx < m_particles.size() && limit > 0; secondParticleIdx++ )
        {
            Particle* secondParticle = m_particles[ secondParticleIdx ];
            m_spontaneousCollisionGenerator->m_particles[ 0 ] = firstParticle;
            m_spontaneousCollisionGenerator->m_particles[ 1 ] = secondParticle;

            int used = m_spontaneousCollisionGenerator->addContact( nextContact, limit );
            limit -= used;
            nextContact += used;
        }
    }

    return m_maxContacts - limit;
}

/**
 * @brief Mise � jour de la physique des particules en fonction du temps �coul� depuis la derni�re frame
 * @param deltaTime 
*/
void Engine::runPhysics( const float& secondsElapsedSincePreviousUpdate)
{
    // Ajout des forces au registre
    for( Particle* particle : m_particles )
    {
        if (particle->getVelocity().norm() < 0.2) //on garde une marge en cas de microrebonds
            particle->setIsStationary(true);
        else
            particle->setIsStationary(false);
        // Gravit�
        m_particleForceRegistry.add( particle, new ParticleGravity( m_gravity ) );

        // Frottements de l'air r�alistes
        if (s_realisticAirResistance)
        {
            m_particleForceRegistry.add(particle, new ParticleAirFriction(getWind()));
        }
    }

    // Mise � jour des forces
    m_particleForceRegistry.updateForces(secondsElapsedSincePreviousUpdate);

    // Nettoyage du registre
    m_particleForceRegistry.clear();

    // Mise � jour physique de chaque particule
    for( Particle* particle : m_particles )
    {
        particle->integrate(secondsElapsedSincePreviousUpdate);
    }

    // Pour �viter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'it�rateur)
    for (Particle* ashFallParticle : m_tempAshFallParticles)
    {
        m_particles.push_back(ashFallParticle);
    }
    m_tempAshFallParticles.clear(); // Apr�s avoir copi� tous les �l�ments de cette liste temporaire, on la vide


    // G�n�ration des collisions
    int usedContacts = generateContacts();

    // Traitement des collisions
    if( usedContacts > 0 )
    {
        if( m_calculateIterations )
        {
            // G�n�ralement on prend nbIterations = 2 * nbCollisions par convention
            m_contactResolver.setIterations( usedContacts * 2 );
        }

        m_contactResolver.resolveContacts( m_contacts, usedContacts, secondsElapsedSincePreviousUpdate);
    }

    // Nettoyage des particules inutiles
    cleanup();
}


/**
 * @brief Supprime les particules sorties de l'�cran ou trop petites
*/
void Engine::cleanup()
{
    std::vector<Particle*>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // on supprime les particules qui ont �t� marqu�es comme "� d�truire" et celles qui sont devenues trop petites (le radius des train�es de cendres diminue automatiquement)
        if( ( *particleIterator )->getRadius() < 0.009 || (*particleIterator)->toBeDestroyed() ) {
            particleIterator = m_particles.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }
    }
}


/**
 * @brief Dessine les particules
*/
void Engine::drawParticles() const
{
    for( Particle* currParticle : m_particles )
    {
        currParticle->draw();
    }
}


/**
 * @brief Calcule une nouvelle valeur al�atoire pour un canal de couleur (R, G ou B) entre 0 et 255, sans d�passer un d�calage �gal � shiftAmount
 * @param colorChannel 
 * @param shiftAmount 
 * @return 
*/
float Engine::randshiftColorChannel( const float& colorChannel, const int& shiftAmount )
{
    float newColorChannel = colorChannel + rand() % ( shiftAmount * 2 + 1 ) - shiftAmount;

    if( newColorChannel < 0 ) newColorChannel = 0;
    if( newColorChannel > 255 ) newColorChannel = 255;

    return newColorChannel;
}


/**
 * @brief Calcule une nouvelle couleur RGB al�atoire � partir d'une couleur existante. Utilise la m�thode randshiftColorChannel.
 * @param color 
 * @param shiftAmount 
 * @return 
*/
Vector3 Engine::randshiftColor( const Vector3& color, const int& shiftAmount )
{
    Vector3 newColor( color );

    newColor.x = randshiftColorChannel( newColor.getX(), shiftAmount );
    newColor.y = randshiftColorChannel( newColor.getY(), shiftAmount );
    newColor.z = randshiftColorChannel( newColor.getZ(), shiftAmount );

    return newColor;
}


/**
 * @brief G�re le comportement des particules face � un clic de souris.
 * @param x 
 * @param y 
 * @return 
*/
bool Engine::clickedParticle( const float& x, const float& y )
{
    bool clicked = false;


    const bool conversionIsFromGraphicToMecanic = false;
    const Vector3 clicGraphique = Vector3({ x, y, 0.0 });
    const Vector3 clicMecanique = s_referential.conversionPositionMecaniqueGraphique(clicGraphique, conversionIsFromGraphicToMecanic);

    const float clicMecaniqueX = clicMecanique.getX();
    const float clicMecaniqueY = clicMecanique.getY();

    std::vector<Particle*>::iterator particleIterator = m_particles.begin();
    while( particleIterator != m_particles.end() && clicked == false )
    {
        const float particlePositionX = (*particleIterator)->getPosition().getX();
        const float particlePositionY = (*particleIterator)->getPosition().getY();
        const float particleRadius = (*particleIterator)->getRadius();

        if( (particlePositionX - particleRadius < clicMecaniqueX && particlePositionX + particleRadius > clicMecaniqueX)
            && (particlePositionY - particleRadius < clicMecaniqueY && particlePositionY + particleRadius > clicMecaniqueY) )
        {
            clicked = true;
            ( *particleIterator )->clicked();
        }
        /*
        if( ( *particleIterator )->toBeDestroyed() )
        {
            particleIterator = m_particles.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }*/
        particleIterator++;
    }

    return clicked;
}


void Engine::showScore(const bool& boolShowScore) const
{
    if (boolShowScore)
    {
        ofSetColor(255, 100, 100);
        ofDrawBitmapString("Score : " + ofToString(score), (Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 })).v3());
    }
}