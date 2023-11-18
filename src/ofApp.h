#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Matrix.h"
#include "Engine.h"
#include "MouseDragger.h"
#include "Collision/GroundContacts.h"
#include "Collision/BlobContact.h"
#include "Blob.h"

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
        void mouseScrolled(ofMouseEventArgs& mouse);
        void windowResized( int w, int h );
        void dragEvent( ofDragInfo dragInfo );
        void gotMessage( ofMessage msg );

        int fps = 0;
        // Interface (GUI)
        ofxPanel m_gui;

        // Pour avoir un champ au lieu d'un slider => utiliser ofxFloatField
        ofParameterGroup m_mainGroup;
    
        ofParameterGroup m_worldForces;
        ofParameter<float> m_gravitySlider;
        ofParameter<bool> m_realisticAirLossToggle;
        ofParameter<float> m_windX_slider;
        ofParameter<float> m_windY_slider;
        ofParameter<float> m_dampingSlider;
        ofParameterGroup m_particleConfig;
        ofParameter<bool> m_isShootingTrigger;
        ofParameter<bool> m_showParticleInfosToggle;
        ofParameter<float> m_impulseSlider;
        ofParameter<float> m_massSlider;
        ofParameter<float> m_radiusSlider;
        ofParameter<ofVec3f> m_angularVelocitySlider;
        ofParameter<bool> m_isFireballToggle;
        ofParameter<ofVec3f> m_colorSlider;
        ofParameter<int> m_colorShiftSlider;

        // Variable qui sert à compter le temps écoulé entre chaques "frames"
        std::chrono::time_point<std::chrono::system_clock> dateOfBeginPreviousUpdate;

        // Générateur de sol
        GroundContacts groundContactGenerator;

        // Générateur de câble pour le comportement câble des ressorts des blobs
        //BlobContact blobContactGenerator;

        
        void initArrays();
        std::pair<glm::vec3, glm::vec3> getShootInfo() const;
        void shootRigidbody(std::pair<glm::vec3, glm::vec3>);

    private:
        MouseDragger draggerReferentialOrigin;
        MouseDragger draggerParticleLauncher;
        MouseDragger draggerSelection;

        // l'indice 0 correspond au clic gauche, le 1 au clic scroll et le 2 au clic droit
        bool boolsMouseButtonPressed[3]; // initialisation à false sur tous les champs par défaut


        bool m_cameraInfoSaved;
        glm::vec3 m_shootPos;
        glm::vec3 m_shootAxis;

        float m_moveSpeed;
        bool m_mustMoveDirections[6];
        Vector3 m_moveDirections[6];

        bool m_canRotateCamera;
        std::pair<int, int> m_mousePos;

};
