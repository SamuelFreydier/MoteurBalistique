#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Matrix.h"
#include "Engine.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void integrate();
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

    // Interface (GUI)
    ofxPanel m_gui;

    // Pour avoir un champ au lieu d'un slider => utiliser ofxFloatField
    ofParameterGroup m_mainGroup;
    
    ofParameterGroup m_worldForces;
    ofParameter<float> m_gravitySlider;
    ofParameter<float> m_dampingSlider;
    ofParameterGroup m_particleConfig;
    ofParameter<bool> m_isShootingTrigger;
    ofParameter<float> m_impulseSlider;
    ofParameter<float> m_massSlider;
    ofParameter<float> m_radiusSlider;
    ofParameter<bool> m_isFireballToggle;
    ofParameter<ofVec3f> m_colorSlider;
    ofParameter<int> m_colorShiftSlider;

    // Calcul du temps passé à chaque frame
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::steady_clock::time_point m_endTime;
};
