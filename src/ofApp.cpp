#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Setup random generator
    srand( time( NULL ) );

    // Setup GUI
    m_EngineConfig.setName("Engine Configuration");
    m_EngineConfig.add(m_minUpdateDelay.set("Minimum Update Delay", 0, 0, 1));

    m_worldForces.setName("World Forces");
    m_worldForces.add(m_gravitySlider.set("Gravity", 9.81, 1, 20));
    m_worldForces.add(m_linearDampingSlider.set("Linear Damping", 0.94, 0.5, 1));
    m_worldForces.add(m_angularDampingSlider.set("Angular Damping", 0.94, 0.5, 1));

    m_RigidbodyConfig.setName("Rigidbody Configuration");
    m_RigidbodyConfig.add(m_impulseSlider.set("Impulse", 20, 1, 500));
    m_RigidbodyConfig.add(m_massSlider.set("Mass", 5, 1, 100));
    m_RigidbodyConfig.add(m_useSpring.set("Create Anchored Springs", false));
    m_RigidbodyConfig.add(m_useInitAngularVelocity.set("Use Initial Angular Velocity", false));
    m_RigidbodyConfig.add(m_angularVelocitySlider.set("Angular velocity", ofVec3f(0.0, 0.0, 0.0), ofVec3f(-100.0, -100.0, -100.0), ofVec3f(100.0, 100.0, 100.0)));
    m_RigidbodyConfig.add(m_colorSlider.set("Color", ofVec3f(200, 50, 50), ofVec3f(0, 0, 0), ofVec3f(255, 255, 255)));

    m_CuboidConfig.setName("Cuboid Configuration");
    m_CuboidConfig.add(m_widthSlider.set("Width", 1, 0.1, 5));
    m_CuboidConfig.add(m_cuboidHeightSlider.set("Height", 1, 0.1, 5));
    m_CuboidConfig.add(m_depthSlider.set("Depth", 1, 0.1, 5));

    m_SpringConfig.setName("Spring Configuration");
    m_SpringConfig.add(m_springContactPoint.set("Contact Point", ofVec3f(0, 0, 0), ofVec3f(-2.5, -2.5, -2.5), ofVec3f(2.5, 2.5, 2.5)));
    m_SpringConfig.add(m_springConstant.set("Spring Constant", 5, 1, 50));
    m_SpringConfig.add(m_springRestLength.set("Rest Length", 1, 1, 100));

    m_mainGroup.add(m_EngineConfig);
    m_mainGroup.add(m_worldForces);
    m_mainGroup.add(m_RigidbodyConfig);
    m_mainGroup.add(m_CuboidConfig);
    m_mainGroup.add(m_SpringConfig);

    m_gui.setup(m_mainGroup);
    m_gui.setPosition(glm::vec3(0, 0, 0));


    // Setup App
    int maxContacts = 100000;
    Engine::getInstance( maxContacts );

    /*for( int i = 0; i < particleCount; i++ )
    {
        Engine::getInstance()->addParticle( new Particle() );
    }*/

    groundContactGenerator.init( &Engine::getInstance()->getParticles(), &Engine::getInstance()->getRigidbodies() );
    //blobContactGenerator.init( &Engine::getInstance()->getBlobs());

    Engine::getInstance()->addContactGenerator( &groundContactGenerator );
    //Engine::getInstance()->addContactGenerator(&blobContactGenerator);

    m_useSpring = false;

    m_rbType = Engine::RigidbodyType::CuboidType;

    // Initialisation des coordonn�es du point d'origine du r�f�rentiel cart�sien
    Engine::getInstance()->getReferential().setPointOrigine(200, ofGetScreenHeight() - 50);

    // Variable qui sert � compter le temps �coul� entre chaques "frames"
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now(); 
    updateTimer = 0;

    m_moveSpeed = 50;
    m_cameraInfoSaved = false;
    m_cameraZoom = 1;
    m_cameraDefaultFov = Engine::getInstance()->getFov();
    initArrays();
    Engine::getInstance()->moveCamera(Vector3(0, 50, 0));
}

//--------------------------------------------------------------
void ofApp::update()
{
    Engine* instance = Engine::getInstance();

    // Nettoyage de la physique
    instance->clearRegisteries();

    // Mise � jour des forces configur�es
    instance->setGravity( Vector3( { 0.0, -m_gravitySlider, 0.0 } ) );
    instance->setLinearDamping( m_linearDampingSlider );
    instance->setAngularDamping( m_angularDampingSlider );

    // Calcul du temps en secondes �coul� depuis la pr�c�dente mise � jour -> essentiel pour un moteur physique r�aliste
    std::chrono::duration<float> elapsedSincePreviousUpdate = std::chrono::system_clock::now() - dateOfBeginPreviousUpdate;
    fps = 1 / elapsedSincePreviousUpdate.count();
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now();

    // Mise � jour physique
    if (updateTimer >= m_minUpdateDelay)
    {
        instance->runPhysics(elapsedSincePreviousUpdate.count());
        updateTimer = 0;
    }
    else
    {
        updateTimer += elapsedSincePreviousUpdate.count();
    }
    

    // D�tection de la s�lection graphique de particules
    if (draggerSelection.isDragging())
    {
        const Vector3 currentMousePosition = Vector3(ofGetMouseX(), ofGetMouseY());
        draggerSelection.setSelectedParticles(instance->selectedParticles(draggerSelection.getStartMousePosition(), currentMousePosition));
    }

    //Pr�paration du d�placement de la cam�ra
    Vector3 cameraMovementDirection(0, 0, 0);

    //Calcul du des axes de d�placements
    for (int i = 0; i < 6; i++)
    {
        if (m_mustMoveDirections[i])
        {
            cameraMovementDirection += m_moveDirections[i];
        }
    }

    instance->moveCamera(cameraMovementDirection * m_moveSpeed * elapsedSincePreviousUpdate.count());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    Engine* instance = Engine::getInstance();

    instance->beginCamera();
    ofEnableDepthTest();    // Test de la profondeur

    instance->draw();


    //Si les infos de la cam�ra ont �t� sauvegard�es, on dessine un c�ne pour repr�senter la position et le regard sauvegard�s
    if (m_cameraInfoSaved)
    {
        //Cr�ation du c�ne et calcul de son orientation
        ofConePrimitive shootIndicator;
        shootIndicator.setGlobalPosition(m_shootPos);
        shootIndicator.lookAt(m_shootPos + m_shootAxis);
        shootIndicator.tiltDeg(90);

        //Dessin de ses ar�tes en noir pour en percevoir sa perspective
        shootIndicator.setScale(0.2);
        ofSetColor(0, 0, 0);
        shootIndicator.drawWireframe();

        //Dessin du c�ne avec ses faces
        shootIndicator.setScale(0.2);
        ofSetColor(25, 25, 200);
        shootIndicator.draw();
    }

    ofDisableDepthTest();
    instance->endCamera();

    m_gui.draw();
    ofSetColor(255); // D�finir la couleur du texte en blanc
    string fpsStr = ofToString(fps) + " fps";
    ofDrawBitmapString(fpsStr, ofGetWindowWidth() - 60, 20); // Dessiner le texte � la position (20, 20)
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{

    switch( key )
    {
        // Nettoyage des particules
        case OF_KEY_DEL:
            Engine::getInstance()->clear();
            break;

        //Sauvegarde des infos de la cam�ra
        case OF_KEY_F7:
            m_cameraInfoSaved = true;

            {
                std::pair<glm::vec3, glm::vec3> cameraInfo = Engine::getInstance()->getCameraInfo();
                m_shootPos = cameraInfo.first;
                m_shootAxis = cameraInfo.second;
            }
            break;

        //Suppression de cette sauvegarde
        case OF_KEY_F8:
            m_cameraInfoSaved = false;
            break;

        //Gestion de d�placement de la cam�ra
        case 'z':
            m_mustMoveDirections[0] = true;
            break;

        case 's':
            m_mustMoveDirections[1] = true;
            break;

        case 'q':
            m_mustMoveDirections[2] = true;
            break;

        case 'd':
            m_mustMoveDirections[3] = true;
            break;

        case 'a':
            m_mustMoveDirections[4] = true;
            break;

        case 'e':
            m_mustMoveDirections[5] = true;
            break;

        case 'x':
            resetCameraZoom();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
    
    switch( key )
    {
        case 'z':
            m_mustMoveDirections[0] = false;
            break;

        case 's':
            m_mustMoveDirections[1] = false;
            break;

        case 'q':
            m_mustMoveDirections[2] = false;
            break;

        case 'd':
            m_mustMoveDirections[3] = false;
            break;

        case 'a':
            m_mustMoveDirections[4] = false;
            break;

        case 'e':
            m_mustMoveDirections[5] = false;
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
    //Rotation de la cam�ra si le clic droit est enfonc�
    if (button == 2)
    {
        std::pair<float, float> diffMousePos = { x - m_mousePos.first, y - m_mousePos.second };
        m_mousePos = { x, y };

        Engine::getInstance()->rotateCamera(-diffMousePos.second / m_cameraZoom, -diffMousePos.first / m_cameraZoom);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    // Clic sur un rigidbody permet de faire spawn un rigidbody li� � celui cliqu� par un ressort
    if (button == 1)    // Clic molette
    {
        std::shared_ptr<Rigidbody> clickedRB = Engine::getInstance()->clickedRigidbody(x, y); 

        if (clickedRB != nullptr)
        {
            clickedRB->setColor(Vector3(0, 255, 0));
        }
    }

    //Sauvegarde des positions de la souris pour la rotation de la cam�ra
    if (button == 2)
    {
        m_mousePos = { x, y };
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    if (button == 0)
    {
        shootRigidbody(getShootInfo());
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{

}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{

}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{

}


void ofApp::mouseScrolled(ofMouseEventArgs& mouse)
{
    float deltaZoom = mouse.scrollY * 0.1;

    if (deltaZoom != 0)
    {
        Engine::getInstance()->changeFovCamera(deltaZoom);
        m_cameraZoom += deltaZoom;
    }
}

/**
* @brief Initialisation des tableaux pour le d�placement de la cam�ra
*/
void ofApp::initArrays()
{
    //Init pour les touches ZS QD AE

    for (int i = 0; i < 6; i++)
    {
        m_mustMoveDirections[i] = false;
    }

    m_moveDirections[0] = Vector3::forward;
    m_moveDirections[1] = Vector3::backward;
    m_moveDirections[2] = Vector3::left;
    m_moveDirections[3] = Vector3::right;
    m_moveDirections[4] = Vector3::down;
    m_moveDirections[5] = Vector3::up;
}


/**
* @brief R�cup�re les infos de la cam�ra dans Engine
*
* @return Une paire de vecteurs contenant sa position et sa direction de regard
*/
std::pair<glm::vec3, glm::vec3> ofApp::getShootInfo() const
{
    std::pair<glm::vec3, glm::vec3> savedShootInfo = { m_shootPos, m_shootAxis };
    return m_cameraInfoSaved ? savedShootInfo : Engine::getInstance()->getCameraInfo();
}

/**
* @brief Tire un rigidbody selon une position et un axe
* @param shootInfo
*/
void ofApp::shootRigidbody(std::pair<glm::vec3, glm::vec3> shootInfo) 
{
    Engine* instance = Engine::getInstance();

    float impulse = m_impulseSlider;
    Vector3 initAngularVelocity;
    std::vector<float> rbParameters;
    std::vector<float> springParameters;

    if (m_useInitAngularVelocity)
    {
        initAngularVelocity = Vector3(m_angularVelocitySlider->x, m_angularVelocitySlider->y, m_angularVelocitySlider->z);
    }

    springParameters.push_back(m_springContactPoint->x);
    springParameters.push_back(m_springContactPoint->y);
    springParameters.push_back(m_springContactPoint->z);
    springParameters.push_back(m_springConstant);
    springParameters.push_back(m_springRestLength);

    // Remplissage de la liste des param�tres en fonction du type de rigidbody
    switch (m_rbType)
    {
        case Engine::RigidbodyType::CubeType:
            //rbParameters.push_back(m_sizeSlider);
            //break;
        case Engine::RigidbodyType::CylinderType:
            //rbParameters.push_back(m_radiusSlider);
            //rbParameters.push_back(m_cylinderHeightSlider);
            //break;
        case Engine::RigidbodyType::CuboidType:
            rbParameters.push_back(m_widthSlider);
            rbParameters.push_back(m_depthSlider);
            rbParameters.push_back(m_cuboidHeightSlider);
            break;
    }

    instance->shootRigidbody(m_massSlider, m_rbType, shootInfo.first, shootInfo.second * impulse, initAngularVelocity, Vector3({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }), rbParameters, m_useSpring, springParameters);
}


void ofApp::resetCameraZoom()
{
    m_cameraZoom = 1;
    Engine::getInstance()->setFov(m_cameraDefaultFov);
}