#pragma once

#include "ofMain.h"
#include "Matrix.h"

class ofApp : public ofBaseApp
{

    public:
    void setup();
    void update();
    void draw();

    void keyPressed( int key );
    void keyReleased( int key );
    void mouseMoved( int x, int y );
    void mouseDragged( int x, int y, int button );
    void mousePressed( int x, int y, int button );
    void mouseReleased( int x, int y, int button );
    void mouseEntered( int x, int y );
    void mouseExited( int x, int y );
    void windowResized( int w, int h );
    void dragEvent( ofDragInfo dragInfo );
    void gotMessage( ofMessage msg );

    Vector moveVec = Vector( { 1, 0, 0, 0 } );
    Vector accelVec = Vector( { 1.0f, 0.0f, 0.0f, 0.0f } );
    Vector freinVec = Vector( { -1.0f, 0.0f, 0.0f, 0.0f } );
    Vector accelVecVert = Vector( { 0.0f, 1.0f, 0.0f, 0.0f } );
    Vector freinVecVert = Vector( { 0.0f, -1.0f, 0.0f, 0.0f } );
    Vector pos = Vector( { 0.0f, 100.0f, 0.0f, 0.0f } );
    ofSpherePrimitive sphere_;
};
