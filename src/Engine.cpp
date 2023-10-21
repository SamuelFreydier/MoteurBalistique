#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
int Engine::s_colorShift = 0;


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
void Engine::shootParticle( const Vector3& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass, const float& radius, const Vector3& color, bool isFireball )
{
    float xVelocity = initialSpeed * cos( initialAngle );
    float yVelocity = initialSpeed * sin( initialAngle );
    float zVelocity = 0;

    // l'axe y etant dirige vers le bas, il faut l'inverser afin de bien lancer la particule
    Vector3 initialVelocity( { xVelocity, -yVelocity, zVelocity } );

    // Id�alement il faudrait plut�t utiliser un design pattern comme une Factory si on pr�voit d'instancier plein de particules diff�rentes, �a serait plus extensible et facile � maintenir sur le long terme
    // Pour la phase 1, �a marche avec juste la boule de feu mais �a deviendra bien plus pertinent au fil du temps
    Particle* newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = new Fireball( mass, radius, initialVelocity, initialPos, color, s_colorShift );
    }
    else
    {
        newParticle = new Particle( mass, radius, initialVelocity, initialPos, color );
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
void Engine::runPhysics( const float& deltaTime )
{
    // Ajout des forces au registre
    for( Particle* particle : m_particles )
    {
        // Gravit�
        m_particleForceRegistry.add( particle, new ParticleGravity( m_gravity ) );
    }

    // Mise � jour des forces
    m_particleForceRegistry.updateForces( deltaTime );

    // Nettoyage du registre
    m_particleForceRegistry.clear();

    // Mise � jour physique de chaque particule
    for( Particle* particle : m_particles )
    {
        particle->integrate( deltaTime );
    }

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

        m_contactResolver.resolveContacts( m_contacts, usedContacts, deltaTime );
    }

    // Nettoyage des particules inutiles
    //cleanup();
}


/**
 * @brief Supprime les particules sorties de l'�cran ou trop petites
*/
void Engine::cleanup()
{
    std::vector<Particle*>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // Si la particule est sortie de l'�cran ou est trop petite => Suppression
        if( ( ( *particleIterator )->getPosition().getX() > ofGetWindowWidth() + 5 || ( *particleIterator )->getPosition().getY() > ofGetWindowHeight() + 5 )
            || ( *particleIterator )->getRadius() <= 3.0 ) {
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
    std::vector<Particle*>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() && clicked == false )
    {
        if( ( (*particleIterator)->getPosition().getX() - ( *particleIterator )->getRadius() < x && ( *particleIterator )->getPosition().getX() + ( *particleIterator )->getRadius() > x )
            && ( ( *particleIterator )->getPosition().getY() - ( *particleIterator )->getRadius() < y && ( *particleIterator )->getPosition().getY() + ( *particleIterator )->getRadius() > y ) )
        {
            clicked = true;
            ( *particleIterator )->clicked();
        }
        if( ( *particleIterator )->toBeDestroyed() )
        {
            particleIterator = m_particles.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }
    }

    return clicked;
}