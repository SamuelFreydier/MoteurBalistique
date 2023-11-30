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

        // Referentiel cartésien
        static Referential s_referential;

        // Arbre permettant de subdiviser l'espace et limiter le nombre de collisions à détecter
        Octree m_octree;

        // Objets principaux manipulés par le moteur
        Rigidbodies m_rigidbodies;
        Particles m_particles;
        Particles m_tempAshFallParticles; // Pour éviter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'itérateur)

        // Registre (associations particule / générateur de force) => Gère gravité/frottements/ressorts
        ForceRegistry m_forceRegistry;

        // Résolveur de collisions
        ParticleContactResolver m_contactResolver;

        // Vrai s'il faut calculer le nombre d'itérations à donner au résolveur de collisions à chaque frame
        bool m_calculateIterations;

        // Générateurs de collisions
        ContactGenerators m_contactGenerators;

        // Générateur de collision spécial (pour les collisions spontanées entre particules)
        std::shared_ptr<ParticleSpontaneousCollision> m_spontaneousCollisionGenerator;

        // Liste des collisions
        std::vector<ParticleContact> m_contacts;

        // Nombre maximal de collisions autorisées
        int m_maxContacts;

        // Gravité mise à jour selon les paramètres utilisateur
        Vector3 m_gravity;

        // Frottements pas réaliste
        static float s_linearDamping;
        static float s_angularDamping;

        // Variation des couleurs
        static int s_colorShift;

        // score : +1 par boule de feu explosée !!
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

        // Appelle les générateurs de collision pour signaler les collisions. Retourne le nombre de collisions générées.
        int generateContacts();
        void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

        // Mise à jour PHYSIQUE des particules
        void runPhysics( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Suppression de toutes les particules et rigidbodies contenues dans Engine
        void clear();

        // Suppression de toutes les forces dans les registres
        void clearRegisteries();

        // Fonction globale de dessin qui regroupe toutes les autres fonctions de dessin de Engine
        void draw();

        // Dessine le plan représentant le sol en Y = 0
        void drawGround() const;

        // Mise à jour GRAPHIQUE des particules / rigidbodies / et forces
        void drawParticles() const;
        void drawRigidbodies() const;
        void drawForces();

        // Renvoie la particule / le rigidbody présent.e à l'endroit du clic souris. Renvoie nullptr si rien n'a été cliqué
        std::shared_ptr<Particle> clickedParticle( const float& x, const float& y );
        std::shared_ptr<Rigidbody> clickedRigidbody(const float& x, const float& y);

        // Renvoie la liste (vector) de particules présentes dans l'aire de sélection de la souris. Renvoie vector de taille 0 si rien n'a été sélectionné
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

        //Déplace la caméra d'un vecteur de déplacement donné selon ses axes locaux
        void moveCamera(Vector3 moveDirection);

        //Tourne la caméra selon ses axes X et Y
        void rotateCamera(float aroundXAxis, float aroundYAxis);

        //Sauvegarde de l'orientation de la caméra
        std::pair<float, float> m_cameraRotation;

        //Renvoie la position et la direction de regard de la caméra
        std::pair<glm::vec3, glm::vec3> getCameraInfo() const;
};

#endif