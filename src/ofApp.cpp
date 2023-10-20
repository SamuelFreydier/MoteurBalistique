#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // Setup random generator
    srand( time( NULL ) );

    // Setup App
    int particleCount = 10;
    Engine::getInstance( particleCount * 100 );

    /*for( int i = 0; i < particleCount; i++ )
    {
        Engine::getInstance()->addParticle( new Particle() );
    }*/

    groundContactGenerator.init( &Engine::getInstance()->getParticles() );
    Engine::getInstance()->addContactGenerator( &groundContactGenerator );


    // Setup GUI
    m_worldForces.setName( "World Forces" );
    m_worldForces.add( m_gravitySlider.set( "Gravity", 9.81, 1, 20 ) );
    m_worldForces.add( m_dampingSlider.set( "Damping", 0.94, 0.5, 1 ) );
    
    m_particleConfig.setName( "Particle Configuration" );
    m_particleConfig.add( m_isShootingTrigger.set( "Is Shooting", true) );
    m_particleConfig.add( m_impulseSlider.set( "Impulse", 200, 1, 500 ) );
    m_particleConfig.add( m_massSlider.set( "Mass", 1, 1, 10 ) );
    m_particleConfig.add( m_radiusSlider.set( "Radius", 30, 3, 60 ) );
    m_particleConfig.add( m_isFireballToggle.set( "Fireball", false ) );
    m_particleConfig.add( m_colorSlider.set( "Color", ofVec3f( 200, 50, 50 ), ofVec3f( 0, 0, 0 ), ofVec3f( 255, 255, 255 ) ) );
    m_particleConfig.add( m_colorShiftSlider.set( "Color Shift", 20, 0, 100 ) );

    m_mainGroup.add( m_worldForces );
    m_mainGroup.add( m_particleConfig );

    m_gui.setup( m_mainGroup );

    // Setup calcul de deltaTime
    m_startTime = std::chrono::steady_clock::now();
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Mise à jour des forces configurées
    Engine::getInstance()->setGravity( Vector3( { 0.0, m_gravitySlider, 0.0 } ) );
    Engine::getInstance()->setDamping( m_dampingSlider );
    Engine::getInstance()->setColorShift( m_colorShiftSlider );

    // Calcul du deltaTime
    m_endTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed{ m_endTime - m_startTime };
    fps =(int)std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    if(fps!=0)
        fps = 1000 / fps;
    m_startTime = m_endTime;

    // Mise à jour physique
    Engine::getInstance()->runPhysics( std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count() / 100.0 );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // Mise à jour graphique
    Engine::getInstance()->drawParticles();

    // Affichage de l'UI
    m_gui.draw();
    ofSetColor(255); // Définir la couleur du texte en blanc
    string fpsStr = ofToString(fps) + " fps";
    ofDrawBitmapString(fpsStr, ofGetWindowWidth()-60, 20); // Dessiner le texte à la position (20, 20)
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

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{

}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    // Si on a cliqué sur une particule, on ne tire pas de particule
    if( Engine::getInstance()->clickedParticle( x, y ) == false && m_isShootingTrigger == true )
    {
        // on determine l'angle de lancer
        float shootingAngle = atan2( ofGetWindowHeight() - y, x );
        // on affiche dans la console l'angle et l'impulsion
        std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << m_impulseSlider << std::endl;

        // on lance la particule avec l'angle et l'impulsion détermines
        Engine::getInstance()->shootParticle( Vector3( { 0.0, static_cast< float >( ofGetWindowHeight() ), 0.0 } ), shootingAngle, m_impulseSlider, m_massSlider, m_radiusSlider, Vector3( { m_colorSlider->x, m_colorSlider->y, m_colorSlider->z } ), m_isFireballToggle );
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    
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
