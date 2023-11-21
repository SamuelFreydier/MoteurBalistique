#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Setup random generator
    srand( time( NULL ) );

    // Setup App
    int maxContacts = 100000;
    Engine::getInstance( maxContacts );

    /*for( int i = 0; i < particleCount; i++ )
    {
        Engine::getInstance()->addParticle( new Particle() );
    }*/

    groundContactGenerator.init( &Engine::getInstance()->getParticles() );
    //blobContactGenerator.init( &Engine::getInstance()->getBlobs());

    Engine::getInstance()->addContactGenerator( &groundContactGenerator );
    //Engine::getInstance()->addContactGenerator(&blobContactGenerator);


    // Setup GUI
    m_worldForces.setName( "World Forces" );
    m_worldForces.add( m_gravitySlider.set( "Gravity", 9.81, 1, 20 ) );
    m_worldForces.add(m_realisticAirLossToggle.set("Realistic air interaction", false));
    m_worldForces.add(m_windX_slider.set("Horizontal wind", 0, -20, 20));
    m_worldForces.add(m_windY_slider.set("Vertical wind", 0, -20, 20));
    m_worldForces.add( m_dampingSlider.set( "Damping", 0.94, 0.5, 1 ) );
    
    m_particleConfig.setName( "Particle Configuration" );
    m_particleConfig.add( m_isShootingTrigger.set( "Is Shooting", true) ); 
    m_particleConfig.add(m_showParticleInfosToggle.set("Show particle infos", true));
    m_particleConfig.add( m_impulseSlider.set( "Impulse", 20, 1, 500 ) );
    m_particleConfig.add( m_massSlider.set( "Mass", 5, 1, 100 ) );
    m_particleConfig.add( m_radiusSlider.set( "Radius", 0.5, 0.01, 2 ) );
    m_particleConfig.add(m_angularVelocitySlider.set("Angular velocity", ofVec3f(0.0, 5.0, 0.0), ofVec3f(0, 0, 0), ofVec3f(100.0, 100.0, 100.0)));
    m_particleConfig.add( m_isFireballToggle.set( "Fireball", false ) );
    m_particleConfig.add( m_colorSlider.set( "Color", ofVec3f( 200, 50, 50 ), ofVec3f( 0, 0, 0 ), ofVec3f( 255, 255, 255 ) ) );
    m_particleConfig.add( m_colorShiftSlider.set( "Color Shift", 20, 0, 100 ) );

    m_mainGroup.add( m_worldForces );
    m_mainGroup.add( m_particleConfig );

    m_gui.setup( m_mainGroup );

    // Initialisation des coordonnées du point d'origine du référentiel cartésien
    Engine::getInstance()->getReferential().setPointOrigine(200, ofGetScreenHeight() - 50);

    // Variable qui sert à compter le temps écoulé entre chaques "frames"
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now(); 

    m_moveSpeed = 50;
    m_cameraInfoSaved = false;
    initArrays();
    Engine::getInstance()->moveCamera(Vector3(0, 50, 0));
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Mise à jour des forces configurées
    Engine::getInstance()->setGravity( Vector3( { 0.0, -m_gravitySlider, 0.0 } ) );
    Engine::getInstance()->setDamping( m_dampingSlider );
    Engine::getInstance()->setColorShift( m_colorShiftSlider );

    // Calcul du temps en secondes écoulé depuis la précédente mise à jour -> essentiel pour un moteur physique réaliste
    std::chrono::duration<float> elapsedSincePreviousUpdate = std::chrono::system_clock::now() - dateOfBeginPreviousUpdate;
    fps = 1 / elapsedSincePreviousUpdate.count();
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now();

    // Mise à jour physique
    Engine::getInstance()->runPhysics(elapsedSincePreviousUpdate.count());

    // Détection de la sélection graphique de particules
    if (draggerSelection.isDragging())
    {
        const Vector3 currentMousePosition = Vector3(ofGetMouseX(), ofGetMouseY());
        draggerSelection.setSelectedParticles( Engine::getInstance()->selectedParticles(draggerSelection.getStartMousePosition(), currentMousePosition));
    }

    Vector3 cameraMovementDirection(0, 0, 0);

    for (int i = 0; i < 6; i++)
    {
        if (m_mustMoveDirections[i])
        {
            cameraMovementDirection += m_moveDirections[i];
        }
    }

    Engine::getInstance()->moveCamera(cameraMovementDirection * m_moveSpeed * elapsedSincePreviousUpdate.count());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    /*
    // Dessiner l'aire de sélection de la souris si il y en a une 
    if (draggerSelection.isDragging())
    {
        draggerSelection.drawSelectionDragger();
    }

    // Dessiner le référentiel graphique
    Engine::getInstance()->getReferential().drawReferential();

    // Dessiner les différentes particules
    Engine::getInstance()->drawParticles();

    // Affichage score si maintien du clic molette
    Engine::getInstance()->showScore(boolsMouseButtonPressed[1]);

    // Affichage lanceur de particule si actif
    if (draggerParticleLauncher.isDragging())
    {
        draggerParticleLauncher.drawDragger();
    }

    // Affichage de l'UI
    m_gui.draw();
    ofSetColor(255); // Définir la couleur du texte en blanc
    string fpsStr = ofToString(fps) + " fps";
    ofDrawBitmapString(fpsStr, ofGetWindowWidth()-60, 20); // Dessiner le texte à la position (20, 20)
    */

    Engine::getInstance()->beginCamera();

    Engine::getInstance()->draw();

    if (m_cameraInfoSaved)
    {
        ofConePrimitive shootIndicator;
        shootIndicator.setGlobalPosition(m_shootPos);
        shootIndicator.lookAt(m_shootPos + m_shootAxis);
        shootIndicator.tiltDeg(90);

        shootIndicator.setScale(0.2);
        ofSetColor(0, 0, 0);
        shootIndicator.drawWireframe();

        shootIndicator.setScale(0.19);
        ofSetColor(25, 25, 200);
        shootIndicator.draw();
    }

    Engine::getInstance()->endCamera();
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

        case OF_KEY_F7:
            m_cameraInfoSaved = true;

            {
                std::pair<glm::vec3, glm::vec3> cameraInfo = Engine::getInstance()->getCameraInfo();
                m_shootPos = cameraInfo.first;
                m_shootAxis = cameraInfo.second;
            }
            break;

        case OF_KEY_F8:
            m_cameraInfoSaved = false;
            break;

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
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
    
    switch( key )
    {
        /*
        // Fusion des particules du blob sélectionné
        case OF_KEY_F1: 
        {
            std::shared_ptr<Particle> clickedParticle = Engine::getInstance()->clickedParticle( ofGetMouseX(), ofGetMouseY() );
            if( clickedParticle )
            {
                Engine::getInstance()->mergeBlobParticles( clickedParticle );
            }
        } break;
        case OF_KEY_F2: 
        {
            std::shared_ptr<Particle> clickedParticle = Engine::getInstance()->clickedParticle( ofGetMouseX(), ofGetMouseY() );
            if( clickedParticle )
            {
                Engine::getInstance()->unmergeBlobParticles( clickedParticle );
            }
        } break;
        */

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
    /*
    if (boolsMouseButtonPressed[0] == false)
    {
        draggerParticleLauncher.draggingIsOver();
    }
    if (boolsMouseButtonPressed[2] == false)
    {
        draggerReferentialOrigin.draggingIsOver();
    }
    */
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
    /*
    if (button == 0) // si dragging du clic gauche 
    {
        if (m_isShootingTrigger)
        {
            if (draggerParticleLauncher.isDragging() == false) // si le draggerParticleLauncher n'est pas encore actif, alors on en créée un nouveau et on fait rien de plus
            {
                draggerParticleLauncher = MouseDragger(Vector3(x, y), m_radiusSlider);
            }
        }
        else
        {

        }
    }
    else if (button == 1)
    {
        if (m_isShootingTrigger)
        {

        }
        else
        {
            if (draggerSelection.isDragging() == false) // si le draggerSelection n'est pas encore actif, alors on en créée un nouveau et on fait rien de plus
            {
                draggerSelection = MouseDragger(Vector3(x, y));
            }
        }
    }
    else if (button == 2) // s'il s'agit d'un dragging du clic droit, alors on applique un dragging à l'origine du référentiel
    {
        if (draggerReferentialOrigin.isDragging())
        {
            Engine::getInstance()->getReferential().dragOrigin({ (float)x, (float)y, 0.0 }, draggerReferentialOrigin.getStartMousePosition(), draggerReferentialOrigin.getStartThingPosition());
        }
        else // si le dragOrigin n'est pas encore actif, alors on en créée un nouveau et on fait rien de plus
        {
            draggerReferentialOrigin = MouseDragger(Vector3(x, y), Engine::getInstance()->getReferential().getOriginPoint());
        }
    }
    */

    if (button == 2)
    {
        std::pair<int, int> diffMousePos = { x - m_mousePos.first, y - m_mousePos.second };
        m_mousePos = { x, y };

        Engine::getInstance()->rotateCamera(-diffMousePos.second, -diffMousePos.first);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    /*
    // Quand un clic est détecté, on modifie le tableau de bools en conséquence
    boolsMouseButtonPressed[button] = true;

    if (m_isShootingTrigger) // Si le mode shooting est activé
    {
        if (button == 0) // si clic gauche on essaie de cliquer sur une particule
        {

        }
        else if (button == 1) // si clic molette 
        {
            
        }
        else if (button == 2) // si clic droit, alors on essaie d'exploser des boules
        {
            std::shared_ptr<Particle> clickedParticle = Engine::getInstance()->clickedParticle(x, y);
            if (clickedParticle != nullptr)
            {
                clickedParticle->clicked();
                Engine::getInstance()->increaseScore();
            }
        }
    }
    else // sinon ( = mode blob)
    {
        if (button == 0) // si clic gauche on essaie de cliquer sur une particule
        {
            const Vector3 mecanicSponePosition = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(Vector3(x, y), false);
            std::shared_ptr<Blob> newBlob = std::make_shared<Blob>(mecanicSponePosition);
            Engine::Particles blobParticles = newBlob->getBlobParticles();

            for (std::shared_ptr<Particle> blobParticle : blobParticles)
            {
                Engine::getInstance()->addParticle(blobParticle);
            }

            Engine::getInstance()->addBlob(newBlob);
        }
        else if (button == 1) // si clic molette 
        {
            std::shared_ptr<Particle> clickedParticle = Engine::getInstance()->clickedParticle(x, y);
            if (clickedParticle != nullptr) // si on a cliqué sur une particule
            {
                Engine::getInstance()->destroyCorruptedBlobs(clickedParticle); // on détruit tous les blobs dont cette particule fait partie mais on ne détruit pas les particules
            }
            else // si on a cliqué sur rien, alors on prend toutes les particules de la map et on les réunit en seul gros blob
            {
                Blob* newBlob = new Blob(Engine::getInstance()->getParticles());
                Engine::getInstance()->addBlob(newBlob);
            }
        }
        else if (button == 2) // si clic droit, alors on essaie d'exploser des boules
        {
            std::shared_ptr<Particle> clickedParticle = Engine::getInstance()->clickedParticle(x, y);
            if (clickedParticle != nullptr)
            {
                clickedParticle->m_destroyedLater = true;
                Engine::getInstance()->increaseScore();
            }
        }
    }
    */

    if (button == 2)
    {
        m_mousePos = { x, y };
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    /*
    // Quand le relâchement d'un clic est détecté, on modifie le tableau de bools en conséquence
    boolsMouseButtonPressed[button] = false;


    if (button == 0) // si relâchement d'un clic gauche, alors on traite le draggerParticleLauncher
    {
        // si le shootingTrigger est bon, alors on lance des particules
        if (m_isShootingTrigger == true)
        {
            if (draggerParticleLauncher.isDragging()) // si draggerParticleLauncher est actif, alors on lance une particule puis on désactive
            {
                // on lance la particule avec la position et la vélocité déterminée
                Vector3 mecanicStartVelocity = Vector3({ 0.0, 0.0, 20.0 });
                if (draggerParticleLauncher.isDraggingBig()) // si le dragging est assez grand, alors on met une vélocité initiale non nulle
                {
                    Vector3 graphicStartVelocity = Vector3(draggerParticleLauncher.getStartMousePosition() - Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 20.0 }));
                    mecanicStartVelocity = Engine::getInstance()->getReferential().conversionVelocityMecaniqueGraphique(graphicStartVelocity, false);
                }
                Vector3 mecanicStartPosition = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(draggerParticleLauncher.getStartMousePosition(), false);
                Engine::getInstance()->shootParticle(mecanicStartPosition, mecanicStartVelocity, draggerParticleLauncher.getParticleMass(), draggerParticleLauncher.getParticleRadius(), Vector3({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }), m_isFireballToggle, m_showParticleInfosToggle);

                //std::cout << draggerParticleLauncher.isDraggingBig() << "\n";
                draggerParticleLauncher.draggingIsOver();
            }
            else // si on relâche un clic gauche sans que draggerParticleLauncher ne se soit activé, alors ça veut dire que c'était un clic simple (pas de drag)
            {
                // On retrouve le clic souris dans le repère mécanique
                Vector3 clicSourisGraphique = Vector3({ (float)x, (float)y, 0.0 });
                Vector3 clicSourisMecanique = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(clicSourisGraphique, false);

                // on determine l'angle de lancer (cliquer en (1,0) mécanique = 0° et (0,1) = 90° car cercle trigo)
                float shootingAngle = atan2(clicSourisMecanique.getY(), clicSourisMecanique.getX());
                // on affiche dans la console l'angle et l'impulsion
                //std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << m_impulseSlider << std::endl;

                // on lance la particule avec l'angle et l'impulsion détermines
                const Vector3 initialVelocity = Vector3({ m_impulseSlider * cos(shootingAngle), m_impulseSlider * sin(shootingAngle), 0.0 });
                Engine::getInstance()->shootParticle(Vector3({ 0.0, m_radiusSlider, 0.0 }), initialVelocity, m_massSlider, m_radiusSlider, Vector3({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }), m_isFireballToggle, m_showParticleInfosToggle);
            }
        }
    }
    else if (button == 1)
    {
        if(m_isShootingTrigger == false) // il faut être en mode blob
        {
            if (draggerSelection.isDragging()) // alors on réunit les particules sélectionnées en un blob
            {
                if (draggerSelection.getSelectedParticles().size() >= 2) // Il faut minimum deux particules pour créer un blob
                {
                    std::shared_ptr<Blob> newBlob = std::make_shared<Blob>(draggerSelection.getSelectedParticles());
                    Engine::getInstance()->addBlob(newBlob);
                }
            }
        }

        draggerSelection.draggingIsOver();
    }    
    else if (button == 2) // si relâchement d'un clic droit, alors on annule le dragging d'origine courant (si il n'est même pas actif, c'est pas grave)
    {
        draggerReferentialOrigin.draggingIsOver();
    }
    */

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
    /*
    //std::cout << "button " << mouse.type << "\n";
    if (boolsMouseButtonPressed[0] == false) // bouton du clic gauche de la souris pas appuyé
    {
        Engine::getInstance()->getReferential().resizeScale(mouse);
    }
    else
    {
        // augmenter masse pendant visée
        if (draggerParticleLauncher.isDragging())
        {
            draggerParticleLauncher.changeParticleMass(mouse);
        }
    }
    */
}


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

std::pair<glm::vec3, glm::vec3> ofApp::getShootInfo() const
{
    std::pair<glm::vec3, glm::vec3> savedShootInfo = { m_shootPos, m_shootAxis };
    return m_cameraInfoSaved ? savedShootInfo : Engine::getInstance()->getCameraInfo();
}


void ofApp::shootRigidbody(std::pair<glm::vec3, glm::vec3> shootInfo) 
{
    Engine::getInstance()->shootRigidbody(shootInfo.first, shootInfo.second * 1, Vector3({ m_angularVelocitySlider->x, m_angularVelocitySlider->y, m_angularVelocitySlider->z }), m_massSlider, m_radiusSlider, Vector3({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }));
}