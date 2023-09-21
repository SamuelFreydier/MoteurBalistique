#include "ofApp.h"

ofApp::~ofApp()
{
    delete m_fireball;
    delete m_ball;
}

//--------------------------------------------------------------
void ofApp::setup()
{
    // Setup GUI
    m_worldForces.setName( "World Forces" );
    m_worldForces.add( m_gravitySlider.set( "Gravity", 9.81, 1, 20 ) );
    m_worldForces.add( m_dampingSlider.set( "Damping", 0.94, 0.5, 1 ) );
    
    m_particleConfig.setName( "Particle Configuration" );
    m_particleConfig.add( m_impulseSlider.set( "Impulse", 100, 1, 500 ) );
    m_particleConfig.add( m_massSlider.set( "Mass", 1, 1, 10 ) );
    m_particleConfig.add( m_hasTrailToggle.set( "Has Trail", false ) );
    m_particleConfig.add( m_colorSlider.set( "Color", ofVec3f( 100, 130, 150 ), ofVec3f( 0, 0, 0 ), ofVec3f( 255, 255, 255 ) ) );

    m_mainGroup.add( m_worldForces );
    m_mainGroup.add( m_particleConfig );

    m_gui.setup( m_mainGroup );

    m_startTime = std::chrono::steady_clock::now();
    ofSetColor( 0, 255, 150 );
    
    m_fireball = new Particle( 3.0, true );
    m_ball = new Particle( 1.0, false );
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Mise à jour des forces configurées
    Engine::getInstance()->setGravity( Vector( { 0.0, m_gravitySlider } ) );
    Engine::getInstance()->setDamping( m_dampingSlider );

    // calcul du delta time
    m_endTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed{ m_endTime - m_startTime };
    m_startTime = m_endTime;

    Engine::getInstance()->update( std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count() / 100.0 );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    Engine::getInstance()->drawParticles();

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

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{

}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    // on determine l'angle de lancer
    float shootingAngle = atan2( ofGetWindowHeight() - y, x );
    // on affiche dans la console l'angle et l'impulsion
    std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << m_impulseSlider << std::endl;
    // on lance la particule avec l'angle et l'impulsion détermines
    Engine::getInstance()->shootParticle( shootingAngle, m_impulseSlider, m_massSlider, m_hasTrailToggle, Vector( { m_colorSlider->x, m_colorSlider->y, m_colorSlider->z } ) );
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
