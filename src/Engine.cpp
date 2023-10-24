#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
bool Engine::s_realisticAirResistance = false;
Vector3 Engine::s_wind = Vector3();
int Engine::s_colorShift = 0;
Referential Engine::s_referential = Referential();


/**
 * @brief Crée un moteur permettant de gérer jusqu'à un maximum de collisions par frame.
 * Il est également possible de spécifier un nombre d'itérations à suivre pour la résolution des collisions.
 * Par défaut => 2 fois le nombre de collisions détectées
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

    // TO DO vider m_blobs

    delete[] m_contacts;
    delete m_spontaneousCollisionGenerator;
}

/**
 * @brief Récupération de l'instance du Singleton
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
 * @brief Tire une particule à une certaine position, avec un certain angle et une certaine pulsion
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
    // Idéalement il faudrait plutôt utiliser un design pattern comme une Factory si on prévoit d'instancier plein de particules différentes, ça serait plus extensible et facile à maintenir sur le long terme
    // Pour la phase 1, ça marche avec juste la boule de feu mais ça deviendra bien plus pertinent au fil du temps
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
 * @brief Appelle chaque générateur de collisions pour signaler leurs collisions
 * @return Nombre de collisions générées
*/
int Engine::generateContacts()
{
    if( m_contacts == nullptr )
    {
        return 0;
    }



    int limit = m_maxContacts;
    ParticleContact* nextContact = m_contacts;

    // On itère dans tous les générateurs de collisions
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

    // On itère pour vérifier les collisions entre toutes les particules
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
 * @brief Mise à jour de la physique des particules en fonction du temps écoulé depuis la dernière frame
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
        // Gravité
        m_particleForceRegistry.add( particle, new ParticleGravity( m_gravity ) );

        // Frottements de l'air réalistes
        if (s_realisticAirResistance)
        {
            m_particleForceRegistry.add(particle, new ParticleAirFriction(getWind()));
        }
    }

    // ajout des forces de ressort assurant l'intégrité des blobs
    for (Blob* blob : m_blobs)
    {
        Blob::Partuples blobParticleAssociations = blob->getParticleAssociations();

        for (Blob::ParticleAssociation_t blobParticleAssociation : blobParticleAssociations)
        {
            // Loi de physique, la somme des interactions entre deux objets est nulle ou un truc du genre.
            m_particleForceRegistry.add(blobParticleAssociation.firstParticle, new ParticleSpring(blobParticleAssociation.secondParticle, 10, 1));
            m_particleForceRegistry.add(blobParticleAssociation.secondParticle, new ParticleSpring(blobParticleAssociation.firstParticle, 10, 1));
        }
        /*
        Particles blobParticles = blob->getBlobParticles();

        Particle* firstParticle = blobParticles[0];
        for (Particle* blobParticle : blobParticles)
        {
            //const float rand1 = (float)(rand() % 1001) / 1000;
            //blobParticle->setColor(Vector3(255 * rand1, 100, 100));

            if (blobParticle != firstParticle)
            {
                m_particleForceRegistry.add(blobParticle, new ParticleSpring(firstParticle, 10, 1));
            }

            int lole = 1;
        }
        */
    }

    // Mise à jour des forces
    m_particleForceRegistry.updateForces(secondsElapsedSincePreviousUpdate);

    // Nettoyage du registre
    m_particleForceRegistry.clear();

    // Mise à jour physique de chaque particule
    for( Particle* particle : m_particles )
    {
        particle->integrate(secondsElapsedSincePreviousUpdate);
    }

    // Pour éviter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'itérateur)
    for (Particle* ashFallParticle : m_tempAshFallParticles)
    {
        m_particles.push_back(ashFallParticle);
    }
    m_tempAshFallParticles.clear(); // Après avoir copié tous les éléments de cette liste temporaire, on la vide

       

    // Génération des collisions
    int usedContacts = generateContacts();

    // Traitement des collisions
    if( usedContacts > 0 )
    {
        if( m_calculateIterations )
        {
            // Généralement on prend nbIterations = 2 * nbCollisions par convention
            m_contactResolver.setIterations( usedContacts * 2 );
        }

        m_contactResolver.resolveContacts( m_contacts, usedContacts, secondsElapsedSincePreviousUpdate);
    }

    // Nettoyage des particules inutiles
    cleanup();
}


/**
 * @brief Supprime les particules sorties de l'écran ou trop petites
*/
void Engine::cleanup()
{
    std::vector<Particle*>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // on supprime les particules qui ont été marquées comme "à détruire" et celles qui sont devenues trop petites (le radius des trainées de cendres diminue automatiquement)
        if( ( *particleIterator )->getRadius() < 0.009 || (*particleIterator)->toBeDestroyed() ) 
        {
            for (Blob* blob : m_blobs) // On l'efface d'abord des blobs dont elle faisait potentiellement partie
            {
                blob->eraseDeadParticle(*particleIterator);
            }
            particleIterator = m_particles.erase( particleIterator ); // il manque un delete du pointeur non ? ce ne sont plus des shared_ptr...
        }
        else
        {
            particleIterator++;
        }
    }
}


void Engine::destroyCorruptedBlobs(Particle* corruptedParticle)
{
    Blobs tempGoodBlobs;

    for (Blob* blob : m_blobs) // On parcourt tous les blobs 
    {
        bool isBlobCorrupted = false;

        for (Particle* blobParticle : blob->getBlobParticles())
        {
            if (blobParticle == corruptedParticle)
            {
                isBlobCorrupted = true;
                break;
            }
        }

        if (isBlobCorrupted == false)
        {
            tempGoodBlobs.push_back(blob);
        }
    }

    m_blobs = tempGoodBlobs;
}


/**
 * @brief Dessine les particules
*/
void Engine::drawParticles() const
{
    for( Particle* currParticle : m_particles )
    {
        currParticle->draw();
        currParticle->isSelected = false;
    }
}


/**
 * @brief Calcule une nouvelle valeur aléatoire pour un canal de couleur (R, G ou B) entre 0 et 255, sans dépasser un décalage égal à shiftAmount
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
 * @brief Calcule une nouvelle couleur RGB aléatoire à partir d'une couleur existante. Utilise la méthode randshiftColorChannel.
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
 * @brief Gère le comportement des particules face à un clic de souris.
 * @param x 
 * @param y 
 * @return 
*/
Particle* Engine::clickedParticle( const float& x, const float& y )
{
    bool clicked = false;
    Particle* clickedParticle = nullptr;


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
            clickedParticle = *particleIterator;
            //( *particleIterator )->clicked();
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

    return clickedParticle;
}


Engine::Particles Engine::selectedParticles(const Vector3& startMousePosition, const Vector3& currentMousePosition)
{
    Particles theSelectedParticles;

    const Vector3 startMecanicMousePosition = getReferential().conversionPositionMecaniqueGraphique(startMousePosition, false);
    const Vector3 currentMecanicMousePosition = getReferential().conversionPositionMecaniqueGraphique(currentMousePosition, false);

    float minX, maxX, minY, maxY;

    if (startMecanicMousePosition.x < currentMecanicMousePosition.x)
    {
        minX = startMecanicMousePosition.x;
        maxX = currentMecanicMousePosition.x;
    }
    else
    {
        minX = currentMecanicMousePosition.x;
        maxX = startMecanicMousePosition.x;
    }


    if (startMecanicMousePosition.y < currentMecanicMousePosition.y)
    {
        minY = startMecanicMousePosition.y;
        maxY = currentMecanicMousePosition.y;
    }
    else
    {
        minY = currentMecanicMousePosition.y;
        maxY = startMecanicMousePosition.y;
    }


    for (Particle* particle : m_particles)
    {
        const float X = particle->getPosition().getX();
        const float Y = particle->getPosition().getY();
        const float radius = particle->getRadius();

        const bool xIsGood = X + radius >= minX && X - radius <= maxX;
        const bool yIsGood = Y + radius >= minY && Y - radius <= maxY;

        if (xIsGood && yIsGood)
        {
            theSelectedParticles.push_back(particle);
        }
    }

    return theSelectedParticles;
}


void Engine::showScore(const bool& boolShowScore) const
{
    if (boolShowScore)
    {
        ofSetColor(255, 100, 100);
        ofDrawBitmapString("Score : " + ofToString(score), (Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 })).v3());
    }
}