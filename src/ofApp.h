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

        ofParameterGroup m_EngineConfig;
        ofParameter<float> m_minUpdateDelay;
    
        ofParameterGroup m_worldForces;
        ofParameter<float> m_gravitySlider;
        ofParameter<float> m_linearDampingSlider;
        ofParameter<float> m_angularDampingSlider;

        ofParameterGroup m_RigidbodyConfig;
        ofParameter<float> m_impulseSlider;
        ofParameter<float> m_massSlider;
        ofParameter<ofVec3f> m_angularVelocitySlider;
        ofParameter<ofVec3f> m_colorSlider;
        ofParameter<bool> m_useSpring;
        ofParameter<bool> m_useInitAngularVelocity;

        ofParameterGroup m_CubeConfig;
        ofParameter<float> m_sizeSlider;

        ofParameterGroup m_CuboidConfig;
        ofParameter<float> m_widthSlider;
        ofParameter<float> m_cuboidHeightSlider;
        ofParameter<float> m_depthSlider;

        ofParameterGroup m_CylinderConfig;
        ofParameter<float> m_radiusSlider;
        ofParameter<float> m_cylinderHeightSlider;

        ofParameterGroup m_SpringConfig;
        ofParameter<ofVec3f> m_springContactPoint;
        ofParameter<float> m_springConstant;
        ofParameter<float> m_springRestLength;

        // Variable qui sert � compter le temps �coul� entre chaques "frames"
        std::chrono::time_point<std::chrono::system_clock> dateOfBeginPreviousUpdate;
        float updateTimer;

        // G�n�rateur de sol
        GroundContacts groundContactGenerator;

        // G�n�rateur de c�ble pour le comportement c�ble des ressorts des blobs
        //BlobContact blobContactGenerator;

        //Fonction pour initialiser les arrays li�s au d�placements de la cam�ra
        void initArrays();

        //Fonction de r�cup�ration des infos de la cam�ra dans Engine
        std::pair<glm::vec3, glm::vec3> getShootInfo() const;

        //Fonction pour tirer un rigidbody depuis une position et dans une direction donn�es
        void shootRigidbody(std::pair<glm::vec3, glm::vec3>);

        void resetCameraZoom();

    private:
        MouseDragger draggerReferentialOrigin;
        MouseDragger draggerParticleLauncher;
        MouseDragger draggerSelection;

        Engine::RigidbodyType m_rbType;

        // l'indice 0 correspond au clic gauche, le 1 au clic scroll et le 2 au clic droit
        bool boolsMouseButtonPressed[3]; // initialisation � false sur tous les champs par d�faut

        //Variables pour sauvegarder la position et la direction de regard de la cam�ra
        bool m_cameraInfoSaved;
        glm::vec3 m_shootPos;
        glm::vec3 m_shootAxis;

        //Variables pour contr�ler les d�placements de la cam�ra
        float m_moveSpeed;
        bool m_mustMoveDirections[6];
        Vector3 m_moveDirections[6];

        //Variables de zoom de la cam�ra
        float m_cameraZoom;
        float m_cameraDefaultFov;

        //Sauvegarde des positions de la souris pour changer l'orientation de la cam�ra
        std::pair<int, int> m_mousePos;
};
