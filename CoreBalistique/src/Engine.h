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

class Engine
{
    public:
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;
        typedef std::vector<std::shared_ptr<Particle>> Particles;
        typedef std::vector<std::shared_ptr<Rigidbody>> Rigidbodies;
        typedef std::vector<std::shared_ptr<AnchoredSpring>> AnchoredSprings;
        //typedef std::vector<std::shared_ptr<Blob>> Blobs;

    private:
        // Singleton
        static Engine* s_engine;

        // Referentiel cartésien
        static Referential s_referential;

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
        static float s_damping;

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

        // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
        void shootRigidbody(const Vector3& initialPos, const Vector3& initialVelocity, const Vector3& initialAngularVelocity, const float& mass, const float& size, const Vector3& color);
   
        // Appelle les générateurs de collision pour signaler les collisions. Retourne le nombre de collisions générées.
        int generateContacts();
        void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

        // Mise à jour PHYSIQUE des particules
        void runPhysics( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Suppression de toutes les particules contenues dans Engine
        void clear();

        // Mise à jour GRAPHIQUE des particules et rigidbodies
        void drawParticles() const;
        void drawRigidbodies() const;

        // Renvoie la particule présente à l'endroit du clic souris. Renvoie nullptr si rien n'a été cliqué
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

        AnchoredSprings& getAnchoredSprings() { return m_anchoredSprings; }
        void addTempAshFallParticles(std::shared_ptr<AnchoredSpring> spring) { m_anchoredSprings.push_back(spring); }

        const Vector3& getGravity() const { return m_gravity; }
        void setGravity( const Vector3& gravity ) { m_gravity = gravity; }

        static const float& getDamping() { return s_damping; }
        static void setDamping( const float& damping ) { s_damping = damping; }

        static const int& getColorShift() { return s_colorShift; }
        static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

        static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
        static Vector3 randshiftColor( const Vector3& color, const int& shiftAmount );

        const int& getScore() const { return score; };
        void increaseScore(const int& points = 1) { score += points; };
        void showScore(const bool& boolShowScore) const;

        void beginCamera() { m_camera.begin(); }
        void endCamera() { m_camera.end(); }
        void moveCamera(Vector3 moveDirection);
        void rotateCamera(float aroundXAxis, float aroundYAxis);
        std::pair<float, float> m_cameraRotation;
        std::pair<glm::vec3, glm::vec3> getCameraInfo() const;

        void draw();
        void drawGround() const;


        //Blobs& getBlobs() { return m_blobs; }
        //void addBlob( std::shared_ptr<Blob> blob );
        //void destroyCorruptedBlobs(std::shared_ptr<Particle> corruptedParticle);

        //void mergeBlobParticles( std::shared_ptr<Particle> selectedParticle );
        //void unmergeBlobParticles( std::shared_ptr<Particle> selectedParticle, float childrenRadius = 2.5f );
};

#endif