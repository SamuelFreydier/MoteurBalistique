#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
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
    m_contacts = std::vector<ParticleContact>( maxContacts, ParticleContact() );

    m_spontaneousCollisionGenerator = std::make_shared<ParticleSpontaneousCollision>();
    m_spontaneousCollisionGenerator->m_restitution = 1.f;

    m_calculateIterations = ( iterations == 0 );

    glm::vec3 cameraOrientation = m_camera.getOrientationEulerDeg();
    m_cameraRotation = { cameraOrientation.x, cameraOrientation.y };
}

Engine::~Engine()
{
    m_particles.clear();
    m_rigidbodies.clear();
    //m_blobs.clear();
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
void Engine::shootRigidbody(const Vector3& initialPos, const Vector3& initialVelocity, const Vector3& initialAngularVelocity, const float& mass, const float& size, const Vector3& color)
{
    // Idéalement il faudrait plutôt utiliser un design pattern comme une Factory si on prévoit d'instancier plein de particules différentes, ça serait plus extensible et facile à maintenir sur le long terme
    // Pour la phase 1, ça marche avec juste la boule de feu mais ça deviendra bien plus pertinent au fil du temps
    std::shared_ptr<Rigidbody> newRB = std::make_shared<RigidbodyCylinder>(size*0.5f,size*2, mass, initialVelocity, initialPos, initialAngularVelocity, color);
    //if( isFireball == true )
    //{
    //    newParticle = std::make_shared<Fireball>( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos, s_colorShift );
    //}
    //else
    //{
    //    newParticle = std::make_shared<Particle>( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos);
    //}

    m_rigidbodies.push_back(newRB);
}

/**
 * @brief Appelle chaque générateur de collisions pour signaler leurs collisions
 * @return Nombre de collisions générées
*/
int Engine::generateContacts()
{
    if( m_contacts.empty() )
    {
        return 0;
    }

    int limit = m_maxContacts;
    std::vector<ParticleContact>& nextContact = m_contacts;
    int contactIndex = 0;

    // On itère dans tous les générateurs de collisions
    for( ContactGenerators::iterator gen = m_contactGenerators.begin(); gen != m_contactGenerators.end(); gen++ )
    {
        int used = ( *gen )->addContact( &nextContact[contactIndex], limit);
        limit -= used;
        contactIndex += used;

        // Plus de collision possible.
        if( limit <= 0 )
        {
            break;
        }
    }

    // On itère pour vérifier les collisions entre toutes les particules
    for( int firstParticleIdx = 0 ; firstParticleIdx < m_particles.size() && limit > 0; firstParticleIdx++  )
    {
        std::shared_ptr<Particle> firstParticle = m_particles[ firstParticleIdx ];
        for( int secondParticleIdx = firstParticleIdx + 1; secondParticleIdx < m_particles.size() && limit > 0; secondParticleIdx++ )
        {
            std::shared_ptr<Particle> secondParticle = m_particles[ secondParticleIdx ];
            m_spontaneousCollisionGenerator->m_particles[ 0 ] = firstParticle;
            m_spontaneousCollisionGenerator->m_particles[ 1 ] = secondParticle;

            int used = m_spontaneousCollisionGenerator->addContact( &nextContact[contactIndex], limit);
            limit -= used;
            contactIndex += used;
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
    // Ajout des forces au registre des particules
    for( std::shared_ptr<Particle>& particle : m_particles )
    {
        if (particle->getVelocity().norm() < 0.2) //on garde une marge en cas de microrebonds
            particle->setIsStationary(true);
        else
            particle->setIsStationary(false);
        // Gravité
        m_forceRegistry.add( particle, std::make_shared<Gravity>( m_gravity ) );
    }

    //Ajout des forces au registre des rigidbody
    int i = 0;
    for (std::shared_ptr<Rigidbody>& rigidbody : m_rigidbodies)
    {
        m_forceRegistry.add(rigidbody, std::make_shared <Gravity>(m_gravity));
        m_forceRegistry.add(rigidbody, m_anchoredSprings[i]);
        ++i;
    }

    // ajout des forces de ressort assurant l'intégrité des blobs
    // for ( std::shared_ptr<Blob>& blob : m_blobs)
    // {
    //     Blob::Partuples blobParticleAssociations = blob->getParticleAssociations();
    // 
    //     for (Blob::ParticleAssociation_t blobParticleAssociation : blobParticleAssociations)
    //     {
    //         m_particleForceRegistry.add(blobParticleAssociation.firstParticle, std::make_shared<ParticleSpring>(blobParticleAssociation.secondParticle, blobParticleAssociation.associationElasticity, blobParticleAssociation.associationRestLength));
    //         m_particleForceRegistry.add(blobParticleAssociation.secondParticle, std::make_shared<ParticleSpring>(blobParticleAssociation.firstParticle, blobParticleAssociation.associationElasticity, blobParticleAssociation.associationRestLength ));
    //     }
    // }

    // Mise à jour des forces
    m_forceRegistry.updateForces(secondsElapsedSincePreviousUpdate);

    // Nettoyage des registres
    m_forceRegistry.clear();

    // Mise à jour physique de chaque rigidbody
    for( std::shared_ptr<Rigidbody> rigidbody : m_rigidbodies )
    {
        rigidbody->calculateDerivedData();
        rigidbody->integrate( secondsElapsedSincePreviousUpdate );
    }

    // Mise à jour physique de chaque particule
    for( std::shared_ptr<Particle> particle : m_particles )
    {
        particle->integrate(secondsElapsedSincePreviousUpdate);
    }

    // Pour éviter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'itérateur)
    for (std::shared_ptr<Particle> ashFallParticle : m_tempAshFallParticles)
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

    /*
    for (std::shared_ptr<Particle>& particle : m_particles)
    {
        std::cout << particle->getPosition().y << std::endl;
        std::cout << particle->getVelocity().y << std::endl;
    }
    */
}


/**
 * @brief Vide les tableaux de particules et blobs
*/
void Engine::clear()
{
    m_particles.clear();
    m_tempAshFallParticles.clear();
    m_rigidbodies.clear();
    //m_blobs.clear();
}

/**
 * @brief Supprime les particules sorties de l'écran ou trop petites
*/
void Engine::cleanup()
{
    std::vector<std::shared_ptr<Particle>>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // on supprime les particules qui ont été marquées comme "à détruire" et celles qui sont devenues trop petites (le radius des trainées de cendres diminue automatiquement)
        if( ( *particleIterator )->getRadius() < 0.009 || (*particleIterator)->toBeDestroyed() ) 
        {
            //for (std::shared_ptr<Blob> blob : m_blobs) // On l'efface d'abord des blobs dont elle faisait potentiellement partie
            //{
            //    blob->eraseDeadParticle(*particleIterator);
            //}
            particleIterator = m_particles.erase( particleIterator ); // il manque un delete du pointeur non ? ce ne sont plus des shared_ptr...
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
    for( const std::shared_ptr<Particle>& currParticle : m_particles )
    {
        currParticle->draw();
        currParticle->isSelected = false;
    }
}


void Engine::drawRigidbodies() const
{
    for (const std::shared_ptr<Rigidbody>& currRigidbody : m_rigidbodies)
    {
        currRigidbody->draw();
        currRigidbody->isSelected = false;
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
std::shared_ptr<Particle> Engine::clickedParticle( const float& x, const float& y )
{
    bool clicked = false;
    std::shared_ptr<Particle> clickedParticle = nullptr;


    const bool conversionIsFromGraphicToMecanic = false;
    const Vector3 clicGraphique = Vector3({ x, y, 0.0 });
    const Vector3 clicMecanique = s_referential.conversionPositionMecaniqueGraphique(clicGraphique, conversionIsFromGraphicToMecanic);

    const float clicMecaniqueX = clicMecanique.getX();
    const float clicMecaniqueY = clicMecanique.getY();

    std::vector<std::shared_ptr<Particle>>::iterator particleIterator = m_particles.begin();
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


/**
 * @brief Renvoie toutes les particules prises dans une zone de sélection entre startMousePosition et currentMousePosition
 * @param startMousePosition 
 * @param currentMousePosition 
 * @return 
*/
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


    for ( std::shared_ptr<Particle> particle : m_particles)
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


/**
 * @brief Affiche le score (nombre de particules détruites à la main)
 * @param boolShowScore 
*/
void Engine::showScore(const bool& boolShowScore) const
{
    if (boolShowScore)
    {
        ofSetColor(255, 100, 100);
        ofDrawBitmapString("Score : " + ofToString(score), (Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 })).v3());
    }
}


std::pair<glm::vec3, glm::vec3> Engine::getCameraInfo() const
{
    glm::vec3 pos = m_camera.getGlobalPosition();
    glm::vec3 lookAxis = m_camera.getLookAtDir();

    return { pos, lookAxis };
}

void Engine::moveCamera(Vector3 moveDirection)
{
    Vector3 cameraPos(m_camera.getGlobalPosition());

    Vector3 cameraMove = m_camera.getXAxis() * moveDirection.x + m_camera.getYAxis() * moveDirection.y - m_camera.getZAxis() * moveDirection.z;

    m_camera.move(cameraMove.v3());
}

void Engine::rotateCamera(float aroundXAxis, float aroundYAxis)
{
    m_cameraRotation = { m_cameraRotation.first + aroundXAxis, m_cameraRotation.second + aroundYAxis };

    float xAngle = m_cameraRotation.first;

    m_cameraRotation.first = (xAngle > 90) ? 90 : (xAngle < -90) ? -90 : xAngle;

    m_camera.setOrientation(glm::vec3(m_cameraRotation.first, m_cameraRotation.second, 0));
}

void Engine::draw()
{
    ofSetColor(200, 200, 200);
    drawGround();
    drawParticles();
    drawRigidbodies();
}

void Engine::drawGround() const
{
    ofPlanePrimitive ground(10000, 10000, 100, 100);
    ground.tiltDeg(90);
    ground.drawWireframe();
}


/**
 * @brief Ajoute un blob à Engine, enlève les particules d'un ancien blob éventuel, et supprime les blobs ainsi vides
 * @param blob
*/
/*void Engine::addBlob(std::shared_ptr<Blob> blob)
{
    // Pour chaque particule du nouveau blob
    for( std::shared_ptr<Particle> blobParticle : blob->getBlobParticles() )
    {
        // On cherche dans tous les blobs existants si la particule existe
        Blobs::iterator existingBlobIterator = m_blobs.begin();
        while( existingBlobIterator != m_blobs.end() )
        {
            Particles::iterator existingBlobParticleIterator = (*existingBlobIterator)->getBlobParticlesMutable().begin();

            // On trouve la particule dans le blob et on la supprime
            while( existingBlobParticleIterator != ( *existingBlobIterator )->getBlobParticles().end() )
            {
                if( (*existingBlobParticleIterator) == blobParticle )
                {
                    existingBlobParticleIterator = ( *existingBlobIterator )->getBlobParticlesMutable().erase( existingBlobParticleIterator );
                    break;
                }
                else
                {
                    existingBlobParticleIterator++;
                }
            }

            // Si le blob n'a plus de particule on le supprime
            if( ( *existingBlobIterator )->getBlobParticlesMutable().empty() )
            {
                existingBlobIterator = m_blobs.erase( existingBlobIterator );
            }
            else
            {
                existingBlobIterator++;
            }
        }
    }

    m_blobs.push_back( blob );
}*/


/**
 * @brief Détruit le blob contenant corruptedParticle
 * @param corruptedParticle
*/
/*void Engine::destroyCorruptedBlobs(std::shared_ptr<Particle> corruptedParticle)
{
    Blobs tempGoodBlobs;

    for (std::shared_ptr<Blob> blob : m_blobs) // On parcourt tous les blobs
    {
        bool isBlobCorrupted = false;

        for ( std::shared_ptr<Particle> blobParticle : blob->getBlobParticles())
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
}*/


/**
 * @brief Fusionne les particules d'un blob en une seule grosse particule
 * @param selectedParticle
*/
/*void Engine::mergeBlobParticles(std::shared_ptr<Particle> selectedParticle)
{
    std::shared_ptr<Blob> blobToMerge = nullptr;

    // Recherche du blob affecté
    Blobs::iterator blobIterator = m_blobs.begin();
    bool blobFound = false;

    while( blobIterator != m_blobs.end() && !blobFound ) // On parcourt tous les blobs
    {
        for( std::shared_ptr<Particle> blobParticle : (*blobIterator)->getBlobParticles() )
        {
            if( blobParticle == selectedParticle )
            {
                blobToMerge = *blobIterator;
                blobFound = true;
                break;
            }
        }

        if( !blobFound )
        {
            blobIterator++;
        }
    }

    // Fusion des particules du blob
    if( blobToMerge )
    {
        // On prépare une nouvelle particule qui engloutit toutes les particules du blob
        std::shared_ptr<Particle> motherParticle = nullptr;
        float motherMass = 0.f;
        float motherRadius = 0.f;
        float surfaceSum = 0.0f;

        Vector3 motherAveragePosition;
        Vector3 motherAverageVelocity;
        Vector3 motherAverageColor;

        int nbChildrenParticles = blobToMerge->getBlobParticles().size();

        // Les associations ne servent plus à rien
        blobToMerge->clearParticleAssociations();

        // On supprime toutes les particules du blob en prenant leurs caractéristiques
        for( std::shared_ptr<Particle> childParticle : blobToMerge->getBlobParticles() )
        {
            motherMass += childParticle->getMass();
            surfaceSum += PI * childParticle->getRadius() * childParticle->getRadius();
            motherAveragePosition += childParticle->getPosition();
            motherAverageVelocity += childParticle->getVelocity();
            motherAverageColor += childParticle->getColor();

            // On lui ordonne de mourir dès que possible
            childParticle->m_destroyedLater = true;
        }

        // Calcul du rayon grâce à la surface totale des particules filles
        motherRadius = sqrt( surfaceSum / PI );

        motherAveragePosition /= nbChildrenParticles;
        motherAverageVelocity /= nbChildrenParticles;
        motherAverageColor /= nbChildrenParticles;

        // Création de la particule mère
        motherParticle = std::make_shared<Particle>( motherMass, motherRadius, motherAverageVelocity, motherAveragePosition, motherAverageColor );

        // Suppression du blob qui n'a plus de raison d'exister
        m_blobs.erase( blobIterator );

        // Ajout de la particule dans Engine
        m_particles.push_back( motherParticle );
    }
}*/

/**
 * @brief Sépare une particule en un ensemble uniforme de blobs de particules
 * @param selectedParticle
 * @param childrenRadius
*/
/*void Engine::unmergeBlobParticles(std::shared_ptr<Particle> selectedParticle, float childrenRadius)
{
    if( selectedParticle )
    {
        Particles newParticles;

        float surfaceToDistribute = PI * selectedParticle->getRadius() * selectedParticle->getRadius();
        int nbParticles = 0;

        float surfaceChild = PI * childrenRadius * childrenRadius;

        // Création des particules filles selon le radius de la mère
        while( surfaceToDistribute / surfaceChild >= 1.f )
        {
            surfaceToDistribute -= surfaceChild;

            newParticles.push_back( std::make_shared<Particle>( 1.f, childrenRadius ) );
            nbParticles++;
        }

        // Nouvelle position selon un angle différent de la sphère
        int childIndex = 0;
        float radianOffset = ( 2 * PI ) / nbParticles;

        // Mise à jour d'autres valeurs intéressantes comme la masse
        for( std::shared_ptr<Particle> childParticle : newParticles )
        {
            childParticle->setMassReverse( selectedParticle->getMass() / nbParticles );
            childParticle->setColor( selectedParticle->getColor() );
            childParticle->setPosition( Vector3( selectedParticle->getPosition().x + cos( childIndex * radianOffset ) * selectedParticle->getRadius() * 2,
                                                 selectedParticle->getPosition().y + sin( childIndex * radianOffset ) * selectedParticle->getRadius() * 2,
                                                 0 ) );
            childParticle->setVelocity( selectedParticle->getVelocity() );
            childIndex++;

            // Ajout de la particule fille dans Engine
            m_particles.push_back( childParticle );
        }

        // Suppression de la particule mère
        Particles::iterator particleIterator = m_particles.begin();
        while( particleIterator != m_particles.end() )
        {
            if( ( *particleIterator ) == selectedParticle )
            {
                m_particles.erase( particleIterator );
                break;
            }
            particleIterator++;
        }

        // Création du blob reliant les filles
        addBlob( std::make_shared<Blob>( newParticles ) );
    }
}*/