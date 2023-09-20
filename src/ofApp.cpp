#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    startTime = std::chrono::steady_clock::now();
    impulse = 0.0f;
    ofSetColor( 0, 255, 150 );

    /*
    accelVec.show();

    Matrix newMatrix(3, 3);
    Matrix secondMatrix( 3, 5 );
    float matrixInit[] = { 2, 5, 6,
                           6, 5, 4,
                           4, 9, 8 };
    float secondMatrixInit[] = { 8, 7, 4, 1, 2,
                                 3, 2, 2, 2, 7,
                                 8, 9, 2, 5, 6 };
    newMatrix.setMatrix( matrixInit );
    newMatrix.show();
    secondMatrix.setMatrix( secondMatrixInit );
    secondMatrix.show();
    Matrix result = newMatrix * secondMatrix;

    result.show();
    */
}

//--------------------------------------------------------------
void ofApp::update()
{
    // calcul du delta time
    endTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed{ endTime - startTime };
    startTime = endTime;
    // a chaque frame où la souris reste appuyee, on incrémente impulse
    if( ofGetMousePressed( OF_MOUSE_BUTTON_LEFT ) )
    {
        impulse += 1.0f;
    }
    Engine::getInstance()->update( std::chrono::duration_cast< std::chrono::milliseconds >( elapsed ).count() / 100.0 );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    Engine::getInstance()->drawParticles();
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
    impulse += 0.1f;
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    // on determine l'angle de lancer
    float shootingAngle = atan2( ofGetWindowHeight() - y, x );
    // on affiche dans la console l'angle et l'impulsion
    std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << impulse << std::endl;
    // on lance la particule avec l'angle et l'impulsion détermines
    Engine::getInstance()->shootParticle( shootingAngle, impulse );
    impulse = 0.0f;
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
