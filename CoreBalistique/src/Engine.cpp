#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_linearDamping = 0.94;
float Engine::s_angularDamping = 0.94;
int Engine::s_colorShift = 0;
Referential Engine::s_referential = Referential();
float Engine::s_rigidbodySelectRadius = 10;


/**
 * @brief Cr�e un moteur permettant de g�rer jusqu'� un maximum de collisions par frame.
 * Il est �galement possible de sp�cifier un nombre d'it�rations � suivre pour la r�solution des collisions.
 * Par d�faut => 2 fois le nombre de collisions d�tect�es
 * @param maxContacts 
 * @param iterations 
*/
Engine::Engine( const int& maxContacts, const int& iterations )
    : m_contactResolver( iterations ), m_maxContacts( maxContacts ), m_octree(Area(Vector3(), Vector3( OCTREE_BOUNDARY_SIZE, OCTREE_BOUNDARY_SIZE, OCTREE_BOUNDARY_SIZE )))
{
    m_contacts = std::vector<Contact>( maxContacts, Contact() );

    m_spontaneousCollisionGenerator = std::make_shared<SpontaneousCollision>();
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
 * @brief Tire un rigidbody � une certaine position, avec un certain angle et une certaine pulsion
 * @param mass 
 * @param rbType 
 * @param initialPos 
 * @param initialVelocity 
 * @param initialAngularVelocity 
 * @param color 
 * @param rbParams 
 * @param useSpring 
 * @param springParams 
*/
void Engine::shootRigidbody(const float& mass, const RigidbodyType& rbType, const Vector3& initialPos, const Vector3& initialVelocity, const Vector3& initialAngularVelocity, const Vector3& color, const std::vector<float>& rbParams, const bool& useSpring, const std::vector<float>& springParams)
{
    // Cr�ation du rigidbody
    std::shared_ptr<Rigidbody> newRB = nullptr;

    switch (rbType)
    {
        case RigidbodyType::CubeType:
            newRB = std::make_shared<RigidbodyCube>(rbParams[0], mass, initialVelocity, initialPos, initialAngularVelocity, color);
            break;
        case RigidbodyType::CylinderType:
            newRB = std::make_shared<RigidbodyCylinder>(rbParams[0], rbParams[1], mass, initialVelocity, initialPos, initialAngularVelocity, color);
            break;
        case RigidbodyType::CuboidType:
            newRB = std::make_shared<RigidbodyCuboid>(rbParams[0], rbParams[1], rbParams[2], mass, initialVelocity, initialPos, initialAngularVelocity, color);
            break;
    }

    // Initialise les donn�es d�riv�es du body
    newRB->calculateDerivedData();

    // Utilisation ou non d'un ressort
    Vector3 rbPos = newRB->getPosition();
    Vector3 springContactPoint(springParams[0], springParams[1], springParams[2]);

    // Ressort ancr�
    std::shared_ptr<AnchoredSpring> anchoredSpring = nullptr;
    if (useSpring) 
    {
        anchoredSpring = std::make_shared<AnchoredSpring>(rbPos, springContactPoint, springParams[3], springParams[4]);
    }

    m_anchoredSprings.push_back(anchoredSpring);
    m_rigidbodies.push_back(newRB);
}

/**
 * @brief Appelle chaque g�n�rateur de collisions pour signaler leurs collisions
 * @return Nombre de collisions g�n�r�es
*/
int Engine::generateContacts()
{
    if( m_contacts.empty() )
    {
        return 0;
    }

    int limit = m_maxContacts;
    std::vector<Contact>& nextContact = m_contacts;
    int contactIndex = 0;

    // On it�re dans tous les g�n�rateurs de collisions
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

    // On it�re pour v�rifier les collisions entre toutes les particules
    for( int firstParticleIdx = 0 ; firstParticleIdx < m_collidingSpheres.size() && limit > 0; firstParticleIdx++  )
    {
        std::cout << "Detected collisions : " << m_collidingSpheres.size() << std::endl;
        std::pair<std::shared_ptr<Rigidbody>, std::shared_ptr<Rigidbody>> collidingCouple = m_collidingSpheres[ firstParticleIdx ];
        std::shared_ptr<Rigidbody> firstRigidbody = collidingCouple.first;
        std::shared_ptr<Rigidbody> secondRigidbody = collidingCouple.second;
        m_spontaneousCollisionGenerator->m_rigidbodies[ 0 ] = firstRigidbody;
        m_spontaneousCollisionGenerator->m_rigidbodies[ 1 ] = secondRigidbody;

        int used = m_spontaneousCollisionGenerator->addContact( &nextContact[ contactIndex ], limit );
        std::cout << "Used after spontaneous add contact : " << used << std::endl;

        limit -= used;
        contactIndex += used;

        if( limit <= 0 )
        {
            break;
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
    // Reset du octree
    m_octree.removeAll();
    m_collidingSpheres.clear();

    // Ajout des forces au registre des particules
    for( std::shared_ptr<Particle>& particle : m_particles )
    {
        if (particle->getVelocity().norm() < 0.2) //on garde une marge en cas de microrebonds
            particle->setIsStationary(true);
        else
            particle->setIsStationary(false);
        // Gravit�
        m_forceRegistry.add( particle, std::make_shared<Gravity>( m_gravity ) );
    }

    // Ajout des forces au registre des rigidbody
    // + Ajout des colliders dans le octree en m�me temps pour �viter de faire un autre parcours
    int i = 0;
    for (std::shared_ptr<Rigidbody>& rigidbody : m_rigidbodies)
    {
        if( rigidbody->getVelocity().norm() < 0.2 ) //on garde une marge en cas de microrebonds
            rigidbody->setStationary( true );
        else
            rigidbody->setStationary( false );

        // Ajout du collider dans le octree
        m_octree.add( rigidbody );

        // Gravit�
        m_forceRegistry.add(rigidbody, std::make_shared <Gravity>(m_gravity));

        // Ressort ancr� si non null pour ce rigidbody
        if (m_anchoredSprings[i])
        {
            m_forceRegistry.add(rigidbody, m_anchoredSprings[i]);
        }

        ++i;
    }

    // Mise � jour des forces
    m_forceRegistry.updateForces(secondsElapsedSincePreviousUpdate);

    // Mise � jour physique de chaque rigidbody
    for( std::shared_ptr<Rigidbody> rigidbody : m_rigidbodies )
    {
        rigidbody->calculateDerivedData();
        rigidbody->integrate( secondsElapsedSincePreviousUpdate );
    }

    // Mise � jour physique de chaque particule
    for( std::shared_ptr<Particle> particle : m_particles )
    {
        particle->integrate(secondsElapsedSincePreviousUpdate);
    }

    // Pour �viter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'it�rateur)
    for (std::shared_ptr<Particle> ashFallParticle : m_tempAshFallParticles)
    {
        m_particles.push_back(ashFallParticle);
    }
    m_tempAshFallParticles.clear(); // Apr�s avoir copi� tous les �l�ments de cette liste temporaire, on la vide

    // Pour d�tecter les collisions on pourra utiliser le octree ici avec la m�thode "findAllIntersections" qui retournera toutes les paires de colliders qui sont en contact
    CollidingSpheres collidingCouples = m_octree.findAllIntersections();

    for( auto collidingCouple : collidingCouples ) 
    {
        m_collidingSpheres.push_back( collidingCouple );
    }

    // G�n�ration des collisions
    int usedContacts = generateContacts();

    // Traitement des collisions
    // TODO : g�rer les collisions des rigidbodies
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
 * @brief Vide les tableaux de particules et blobs
*/
void Engine::clear()
{
    m_particles.clear();
    m_tempAshFallParticles.clear();
    m_rigidbodies.clear();
    //m_blobs.clear();
}

void Engine::clearRegisteries()
{
    m_forceRegistry.clear();
}

/**
 * @brief Supprime les particules sorties de l'�cran ou trop petites
*/
void Engine::cleanup()
{
    std::vector<std::shared_ptr<Particle>>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // on supprime les particules qui ont �t� marqu�es comme "� d�truire" et celles qui sont devenues trop petites (le radius des train�es de cendres diminue automatiquement)
        if( ( *particleIterator )->getRadius() < 0.009 || (*particleIterator)->toBeDestroyed() ) 
        {
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

/**
 * @brief Dessine les rigidbodies
*/
void Engine::drawRigidbodies() const
{
    for (const std::shared_ptr<Rigidbody>& currRigidbody : m_rigidbodies)
    {
        currRigidbody->draw();
        currRigidbody->isSelected = false;
    }
}

/**
 * @brief Dessine les forces pouvant �tre repr�sent�es par une ligne (springs et bungee)
 * 
 * Dessine seulement les force sur les rigidbodies
*/
void Engine::drawForces()
{
    m_forceRegistry.drawForces();
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
        }
        particleIterator++;
    }

    return clickedParticle;
}

/**
 * @brief G�re le comportement des rigidbodies face � un clic de souris.
 * @param x
 * @param y
 * @return
*/
std::shared_ptr<Rigidbody> Engine::clickedRigidbody(const float& x, const float& y)
{
    typedef std::shared_ptr<Rigidbody> RigidbodyPtr;

    RigidbodyPtr clickedRigidbody = nullptr;
    float rigidbodyDepth = 0;

    for (std::vector<RigidbodyPtr>::iterator rigidbodyIterator = m_rigidbodies.begin(); rigidbodyIterator != m_rigidbodies.end(); rigidbodyIterator++)
    {
        glm::vec3 screenRbCoords = m_camera.worldToScreen((*rigidbodyIterator)->getPosition().v3());
        //Si screenRbCoords > 1, alors le rb est derri�re la cam�ra

        //std::cout << screenRigidbodyCoords.x << " " << screenRigidbodyCoords.y << " " << screenRigidbodyCoords.z << std::endl;

        if (ofInRange(screenRbCoords.x - x, -s_rigidbodySelectRadius, s_rigidbodySelectRadius) &&
            ofInRange(screenRbCoords.y - y, -s_rigidbodySelectRadius, s_rigidbodySelectRadius) &&
            screenRbCoords.z < 1
        )
        {
            if (screenRbCoords.z < rigidbodyDepth || clickedRigidbody == nullptr)
            {
                clickedRigidbody = *rigidbodyIterator;
                rigidbodyDepth = screenRbCoords.z;
            }
        }
    }

    return clickedRigidbody;
}

/**
 * @brief Renvoie toutes les particules prises dans une zone de s�lection entre startMousePosition et currentMousePosition
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
 * @brief Affiche le score (nombre de particules d�truites � la main)
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

/**
 * @brief Renvoie les informations de position et de regard de la cam�ra
 * 
 * @return Une paire contenant les vecteurs de position et de regard
 */
std::pair<glm::vec3, glm::vec3> Engine::getCameraInfo() const
{
    glm::vec3 pos = m_camera.getGlobalPosition();
    glm::vec3 lookAxis = m_camera.getLookAtDir();

    return { pos, lookAxis };
}

/**
 * @brief D�place la cam�ra d'un vecteur de d�placement selon ses axes locaux
 * @param moveDirection
 */
void Engine::moveCamera(Vector3 moveDirection)
{
    Vector3 cameraPos(m_camera.getGlobalPosition());
    Vector3 cameraMove = m_camera.getXAxis() * moveDirection.x + m_camera.getYAxis() * moveDirection.y - m_camera.getZAxis() * moveDirection.z;

    m_camera.move(cameraMove.v3());
}

/**
 * @brief Tourne la cam�ra selon ses axes X et Y
 * @param aroundXAxis
 * @param aroundYAxis
 */
void Engine::rotateCamera(float aroundXAxis, float aroundYAxis)
{
    m_cameraRotation = { m_cameraRotation.first + aroundXAxis, m_cameraRotation.second + aroundYAxis };

    float xAngle = m_cameraRotation.first;

    m_cameraRotation.first = (xAngle > 90) ? 90 : (xAngle < -90) ? -90 : xAngle;

    m_camera.setOrientation(glm::vec3(m_cameraRotation.first, m_cameraRotation.second, 0));
}

/**
* @brief Regroupe toutes les autres fonctions de dessin dans Engine
*/
void Engine::draw()
{
    ofSetColor(200, 200, 200);
    drawGround();
    drawParticles();
    drawRigidbodies();
    drawForces();
    
    // Dessine une repr�sentation visuelle des subdivisions de l'espace
    m_octree.draw();
}

/**
* @brief Dessine un plan repr�sentant le sol en Y = 0;
*/
void Engine::drawGround() const
{
    ofPlanePrimitive ground(10000, 10000, 100, 100);
    ground.tiltDeg(90);
    ground.drawWireframe();
}