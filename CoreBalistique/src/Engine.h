#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleForceGenerators/ParticleForceRegistry.h"
#include "ParticleForceGenerators/ParticleGravity.h"
#include "ParticleForceGenerators/ParticleAirFriction.h"
#include "ParticleForceGenerators/ParticleAnchoredSpring.h"
#include "ParticleForceGenerators/ParticleSpring.h"
#include "Fireball.h"
#include "Rigidbody.h"
#include "RigidbodyCube.h"
#include "RigidbodyCuboid.h"
#include "RigidbodyCylinder.h"
#include "Collision/ParticleContactResolver.h"
#include "Collision/ParticleContactGenerator.h"
#include "Collision/ParticleSpontaneousCollision.h"
#include "Collision/ParticleCable.h"
#include "Referential.h"
#include "Octree.h"

#define OCTREE_BOUNDARY_SIZE 200

class Engine
{
    public:
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;
        typedef std::vector<std::shared_ptr<Particle>> Particles;
        typedef std::vector<std::shared_ptr<Rigidbody>> Rigidbodies;
        typedef std::vector<std::shared_ptr<AnchoredSpring>> AnchoredSprings;
        typedef std::vector<std::shared_ptr<Spring>> Springs;
        //typedef std::vector<std::shared_ptr<Blob>> Blobs;

        enum RigidbodyType
        {
            CubeType,
            CylinderType,
            CuboidType
        };

    private:
        // Singleton
        static Engine* s_engine;

        // Referentiel cart�sien
        static Referential s_referential;

        // Arbre permettant de subdiviser l'espace et limiter le nombre de collisions � d�tecter
        Octree m_octree;

        // Objets principaux manipul�s par le moteur
        Rigidbodies m_rigidbodies;
        Particles m_particles;
        Particles m_tempAshFallParticles; // Pour �viter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'it�rateur)

        // Registre (associations particule / g�n�rateur de force) => G�re gravit�/frottements/ressorts
        ForceRegistry m_forceRegistry;

        // R�solveur de collisions
        ParticleContactResolver m_contactResolver;

        // Vrai s'il faut calculer le nombre d'it�rations � donner au r�solveur de collisions � chaque frame
        bool m_calculateIterations;

        // G�n�rateurs de collisions
        ContactGenerators m_contactGenerators;

        // G�n�rateur de collision sp�cial (pour les collisions spontan�es entre particules)
        std::shared_ptr<ParticleSpontaneousCollision> m_spontaneousCollisionGenerator;

        // Liste des collisions
        std::vector<ParticleContact> m_contacts;

        // Nombre maximal de collisions autoris�es
        int m_maxContacts;

        // Gravit� mise � jour selon les param�tres utilisateur
        Vector3 m_gravity;

        // Frottements pas r�aliste
        static float s_linearDamping;
        static float s_angularDamping;

        // Variation des couleurs
        static int s_colorShift;

        // score : +1 par boule de feu explos�e !!
        int score = 0;

        ofCamera m_camera;

        AnchoredSprings m_anchoredSprings;

        //Blobs m_blobs;

        static float s_rigidbodySelectRadius;

    protected:
        Engine(const int& maxContacts, const int& iterations = 0);
        ~Engine();

    public:
        Engine( Engine& ) = delete;
        void operator=( const Engine& ) = delete;

        static Engine* getInstance( const int& maxContacts = 0, const int& iterations = 0 );

        static Referential& getReferential() { return s_referential; };

        // Tire un nouveau rigidbody
        void shootRigidbody(const float& mass, const RigidbodyType& rbType, const Vector3& initialPos, const Vector3& initialVelocity, const Vector3& initialAngularVelocity, const Vector3& color, const std::vector<float>& rbParams, const bool& useSpring = false, const std::vector<float>& springParams = std::vector<float>());

        // Appelle les g�n�rateurs de collision pour signaler les collisions. Retourne le nombre de collisions g�n�r�es.
        int generateContacts();
        void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

        // Mise � jour PHYSIQUE des particules
        void runPhysics( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Suppression de toutes les particules et rigidbodies contenues dans Engine
        void clear();

        // Suppression de toutes les forces dans les registres
        void clearRegisteries();

        // Fonction globale de dessin qui regroupe toutes les autres fonctions de dessin de Engine
        void draw();

        // Dessine le plan repr�sentant le sol en Y = 0
        void drawGround() const;

        // Mise � jour GRAPHIQUE des particules / rigidbodies / et forces
        void drawParticles() const;
        void drawRigidbodies() const;
        void drawForces();

        // Renvoie la particule / le rigidbody pr�sent.e � l'endroit du clic souris. Renvoie nullptr si rien n'a �t� cliqu�
        std::shared_ptr<Particle> clickedParticle( const float& x, const float& y );
        std::shared_ptr<Rigidbody> clickedRigidbody(const float& x, const float& y);

        // Renvoie la liste (vector) de particules pr�sentes dans l'aire de s�lection de la souris. Renvoie vector de taille 0 si rien n'a �t� s�lectionn�
        Particles selectedParticles(const Vector3& startMousePosition, const Vector3& currentMousePosition);

        Rigidbodies& getRigidbodies() { return m_rigidbodies; }
        void addRigidbody( std::shared_ptr<Rigidbody> rigidbody ) { m_rigidbodies.push_back( rigidbody ); }

        Particles& getParticles() { return m_particles; }
        void addParticle( std::shared_ptr<Particle> particle ) { m_particles.push_back( particle ); }

        Particles& getTempAshFallParticles() { return m_tempAshFallParticles; }
        void addTempAshFallParticles( std::shared_ptr<Particle> particle) { m_tempAshFallParticles.push_back(particle); }

        ForceRegistry& getForceRegistry() { return m_forceRegistry; }

        const Vector3& getGravity() const { return m_gravity; }
        void setGravity( const Vector3& gravity ) { m_gravity = gravity; }

        static const float& getLinearDamping() { return s_linearDamping; }
        static void setLinearDamping(const float& damping) { s_linearDamping = damping; }

        static const float& getAngularDamping() { return s_angularDamping; }
        static void setAngularDamping(const float& damping) { s_angularDamping = damping; }

        static const int& getColorShift() { return s_colorShift; }
        static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

        static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
        static Vector3 randshiftColor( const Vector3& color, const int& shiftAmount );

        const int& getScore() const { return score; };
        void increaseScore(const int& points = 1) { score += points; };
        void showScore(const bool& boolShowScore) const;

        void beginCamera() { m_camera.begin(); }
        void endCamera() { m_camera.end(); }

        //D�place la cam�ra d'un vecteur de d�placement donn� selon ses axes locaux
        void moveCamera(Vector3 moveDirection);

        //Tourne la cam�ra selon ses axes X et Y
        void rotateCamera(float aroundXAxis, float aroundYAxis);

        //Sauvegarde de l'orientation de la cam�ra
        std::pair<float, float> m_cameraRotation;

        //Renvoie la position et la direction de regard de la cam�ra
        std::pair<glm::vec3, glm::vec3> getCameraInfo() const;
};

#endif