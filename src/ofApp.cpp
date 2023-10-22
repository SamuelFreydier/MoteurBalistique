#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Setup random generator
    srand( time( NULL ) );

    // Setup GUI
    m_worldForces.setName( "World Forces" );
    m_worldForces.add( m_gravitySlider.set( "Gravity", 9.81, 1, 20 ) );
    m_worldForces.add(m_realisticAirLossToggle.set("Realistic air resistance", false));
    m_worldForces.add( m_dampingSlider.set( "Damping", 0.94, 0.5, 1 ) );
    
    m_particleConfig.setName( "Particle Configuration" );
    m_particleConfig.add( m_isShootingTrigger.set( "Is Shooting", true) ); 
    m_particleConfig.add(m_showParticleInfosToggle.set("Show particle infos", true));
    m_particleConfig.add( m_impulseSlider.set( "Impulse", 20, 1, 500 ) );
    m_particleConfig.add( m_massSlider.set( "Mass", 5, 1, 100 ) );
    m_particleConfig.add( m_radiusSlider.set( "Radius", 0.5, 0.01, 2 ) );
    m_particleConfig.add( m_isFireballToggle.set( "Fireball", false ) );
    m_particleConfig.add( m_colorSlider.set( "Color", ofVec3f( 200, 50, 50 ), ofVec3f( 0, 0, 0 ), ofVec3f( 255, 255, 255 ) ) );
    m_particleConfig.add( m_colorShiftSlider.set( "Color Shift", 20, 0, 100 ) );

    m_mainGroup.add( m_worldForces );
    m_mainGroup.add( m_particleConfig );

    m_gui.setup( m_mainGroup );

    // Initialisation des coordonn�es du point d'origine du r�f�rentiel cart�sien
    Engine::getInstance()->getReferential().setPointOrigine(200, ofGetScreenHeight() - 50);

    // Variable qui sert � compter le temps �coul� entre chaques "frames"
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now(); 
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Mise � jour des forces configur�es
    Engine::getInstance()->setGravity( Vector( { 0.0, -m_gravitySlider, 0.0 } ) );
    Engine::getInstance()->setDamping( m_dampingSlider );
    Engine::getInstance()->setRealisticAirResistance(m_realisticAirLossToggle);
    Engine::getInstance()->setColorShift( m_colorShiftSlider );

    // Calcul du temps en secondes �coul� depuis la pr�c�dente mise � jour -> essentiel pour un moteur physique r�aliste
    std::chrono::duration<float> elapsedSincePreviousUpdate = std::chrono::system_clock::now() - dateOfBeginPreviousUpdate;
    dateOfBeginPreviousUpdate = std::chrono::system_clock::now();

    // Mise � jour physique
    Engine::getInstance()->update(elapsedSincePreviousUpdate.count());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // Dessiner le r�f�rentiel cart�sien
    Engine::getInstance()->getReferential().drawReferential();

    // Mise � jour graphique
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
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{

}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
    if (boolsMouseButtonPressed[0] == false)
    {
        draggerParticleLauncher.draggingIsOver();
    }
    if (boolsMouseButtonPressed[2] == false)
    {
        draggerReferentialOrigin.draggingIsOver();
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
    if (button == 0) // si dragging du clic gauche 
    {
        if (draggerParticleLauncher.isDragging())
        {
            //Engine::getInstance()->getReferential().dragOrigin({ (float)x, (float)y, 0.0 }, draggerParticleLauncher.getStartMousePosition(), draggerParticleLauncher.getStartThingPosition());
        }
        else // si le draggerParticleLauncher n'est pas encore actif, alors on en cr��e un nouveau et on fait rien de plus
        {
            draggerParticleLauncher = MouseDragger(Vector({ (float)x, (float)y, 0.0 }), m_radiusSlider);
        }
    }
    else if (button == 1)
    {

    }
    else if (button == 2) // s'il s'agit d'un dragging du clic droit, alors on applique un dragging � l'origine du r�f�rentiel
    {
        if (draggerReferentialOrigin.isDragging())
        {
            Engine::getInstance()->getReferential().dragOrigin({ (float)x, (float)y, 0.0 }, draggerReferentialOrigin.getStartMousePosition(), draggerReferentialOrigin.getStartThingPosition());
        }
        else // si le dragOrigin n'est pas encore actif, alors on en cr��e un nouveau et on fait rien de plus
        {
            draggerReferentialOrigin = MouseDragger(Vector({ (float)x, (float)y, 0.0 }), Engine::getInstance()->getReferential().getPointOrigine());
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    // Quand un clic est d�tect�, on modifie le tableau de bools en cons�quence
    boolsMouseButtonPressed[button] = true;


    if (button == 0) // si clic gauche on essaie de cliquer sur une particule
    {

    }
    else if (button == 1) // si clic molette 
    {

    }
    else if (button == 2) // si clic droit, alors on essaie d'exploser des boules
    {
        const bool particuleTouchee = Engine::getInstance()->clickedParticle(x, y);
        if (particuleTouchee)
        {
            Engine::getInstance()->increaseScore();
        }
    }


}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    // Quand le rel�chement d'un clic est d�tect�, on modifie le tableau de bools en cons�quence
    boolsMouseButtonPressed[button] = false;


    if (button == 0) // si rel�chement d'un clic gauche, alors on traite le draggerParticleLauncher
    {
        // si le shootingTrigger est bon, alors on lance des particules
        if (m_isShootingTrigger == true)
        {
            if (draggerParticleLauncher.isDragging()) // si draggerParticleLauncher est actif, alors on lance une particule puis on d�sactive
            {
                // on lance la particule avec la position et la v�locit� d�termin�e
                Vector mecanicStartVelocity = Vector({ 0.0, 0.0, 0.0 });
                if (draggerParticleLauncher.isDraggingBig()) // si le dragging est assez grand, alors on met une v�locit� initiale non nulle
                {
                    Vector graphicStartVelocity = Vector(draggerParticleLauncher.getStartMousePosition() - Vector({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 }));
                    mecanicStartVelocity = Engine::getInstance()->getReferential().conversionVelocityMecaniqueGraphique(graphicStartVelocity, false);
                }
                Vector mecanicStartPosition = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(draggerParticleLauncher.getStartMousePosition(), false);
                Engine::getInstance()->shootParticle(mecanicStartPosition, mecanicStartVelocity, draggerParticleLauncher.getParticleMass(), draggerParticleLauncher.getParticleRadius(), Vector({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }), m_isFireballToggle, m_showParticleInfosToggle);

                //std::cout << draggerParticleLauncher.isDraggingBig() << "\n";
                draggerParticleLauncher.draggingIsOver();
            }
            else // si on rel�che un clic gauche sans que draggerParticleLauncher ne se soit activ�, alors �a veut dire que c'�tait un clic simple (pas de drag)
            {
                // On retrouve le clic souris dans le rep�re m�canique
                Vector clicSourisGraphique = Vector({ (float)x, (float)y, 0.0 });
                Vector clicSourisMecanique = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(clicSourisGraphique, false);

                // on determine l'angle de lancer (cliquer en (1,0) m�canique = 0� et (0,1) = 90� car cercle trigo)
                float shootingAngle = atan2(clicSourisMecanique.getY(), clicSourisMecanique.getX());
                // on affiche dans la console l'angle et l'impulsion
                std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << m_impulseSlider << std::endl;

                // on lance la particule avec l'angle et l'impulsion d�termines
                const Vector initialVelocity = Vector({ m_impulseSlider * cos(shootingAngle), m_impulseSlider * sin(shootingAngle), 0.0 });
                Engine::getInstance()->shootParticle(Vector({ 0.0, m_radiusSlider, 0.0 }), initialVelocity, m_massSlider, m_radiusSlider, Vector({ m_colorSlider->x, m_colorSlider->y, m_colorSlider->z }), m_isFireballToggle, m_showParticleInfosToggle);
            }
        }
    }
    else if (button == 1)
    {

    }    
    else if (button == 2) // si rel�chement d'un clic droit, alors on annule le dragging d'origine courant (si il n'est m�me pas actif, c'est pas grave)
    {
        draggerReferentialOrigin.draggingIsOver();
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
    //std::cout << "button " << mouse.type << "\n";
    if (boolsMouseButtonPressed[0] == false) // bouton du clic gauche de la souris pas appuy�
    {
        Engine::getInstance()->getReferential().resizeScale(mouse);
    }
    else
    {
        // augmenter masse pendant vis�e
        if (draggerParticleLauncher.isDragging())
        {
            draggerParticleLauncher.changeParticleMass(mouse);
        }
    }
}
