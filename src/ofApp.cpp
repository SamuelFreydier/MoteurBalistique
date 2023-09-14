#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetColor( 0, 255, 150 );
    sphere_ = ofSpherePrimitive();

    accelVec.show();

    Matrix newMatrix( 3, 3 );
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


}

//--------------------------------------------------------------
void ofApp::update()
{

    pos = pos + moveVec;
    sphere_.setPosition( pos.v3() );

}

//--------------------------------------------------------------
void ofApp::draw()
{
    sphere_.draw();
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
