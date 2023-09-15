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
    endTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed{ endTime - startTime };
    startTime = endTime;
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
    switch( key ) {
        case 'd':
            if( moveVec.getX() < 3 ) {
                moveVec = moveVec + accelVec;
            }

            break;
        case 'q':
            if( moveVec.getX() > -3 ) {
                moveVec = moveVec + freinVec;
            }
            break;
        case 'z':
            if( moveVec.getY() > -2 ) {
                moveVec = moveVec + freinVecVert;
            }
            break;
        case 's':
            if( moveVec.getY() < 2 ) {
                moveVec = moveVec + accelVecVert;
            }
            break;
        default:
            break;
    }

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
    float shootingAngle = atan2( ofGetWindowHeight() - y, x );

    std::cout << "Shooting Angle : " << shootingAngle << " / Impulse : " << impulse << std::endl;

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
