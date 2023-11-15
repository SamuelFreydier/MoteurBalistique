#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleForceGenerators/ParticleForceRegistry.h"
#include "ParticleForceGenerators/ParticleGravity.h"
#include "ParticleForceGenerators/ParticleAirFriction.h"
#include "ParticleForceGenerators/ParticleSpring.h"
#include "Fireball.h"
#include "Blob.h"
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
        typedef std::vector<std::shared_ptr<Blob>> Blobs;

    private:
        // Singleton
        static Engine* s_engine;

        // Referentiel cart�sien
        static Referential s_referential;

        Particles m_particles;
        Particles m_tempAshFallParticles; // Pour �viter que le Integrate() d'une fireball ne modifie m_particles en droppant des ashfall ce qui fait planter le programme (bug d'it�rateur)
        
        Blobs m_blobs;

        // Registre (associations particule / g�n�rateur de force) => G�re gravit�/frottements/ressorts
        ParticleForceRegistry m_particleForceRegistry;

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
        static float s_damping;

        // Frottement air r�aliste
        static bool s_realisticAirResistance;

        // Vent
        static Vector3 s_wind;

        // Variation des couleurs
        static int s_colorShift;

        // score : +1 par boule de feu explos�e !!
        int score = 0;

        ofCamera m_camera;

    protected:
        Engine(const int& maxContacts, const int& iterations = 0);
        ~Engine();

    public:
        Engine( Engine& ) = delete;
        void operator=( const Engine& ) = delete;

        static Engine* getInstance( const int& maxContacts = 0, const int& iterations = 0 );

        static Referential& getReferential() { return s_referential; };

        

        static const bool& getRealisticAirResistance() { return s_realisticAirResistance; }
        static void setRealisticAirResistance(const bool& newRealisticAirResistance) { s_realisticAirResistance = newRealisticAirResistance; }

        static const Vector3& getWind() { return s_wind; }
        static void setWind(const Vector3& newWind) { s_wind = newWind; }

        

        // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
        void shootParticle( const Vector3& initialPos, const Vector3& initialVelocity, const float& mass = 1.0, const float& radius = 1.0, const Vector3& color = Vector3( { 255, 0, 0 } ), bool isFireball = false, bool m_showParticleInfos = false);
   
        // Appelle les g�n�rateurs de collision pour signaler les collisions. Retourne le nombre de collisions g�n�r�es.
        int generateContacts();
        void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

        // Mise � jour PHYSIQUE des particules
        void runPhysics( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Suppression de toutes les particules contenues dans Engine
        void clear();

        // Mise � jour GRAPHIQUE des particules
        void drawParticles() const;

        // Renvoie la particule pr�sente � l'endroit du clic souris. Renvoie nullptr si rien n'a �t� cliqu�
        std::shared_ptr<Particle> clickedParticle( const float& x, const float& y );

        // Renvoie la liste (vector) de particules pr�sentes dans l'aire de s�lection de la souris. Renvoie vector de taille 0 si rien n'a �t� s�lectionn�
        Particles selectedParticles(const Vector3& startMousePosition, const Vector3& currentMousePosition);

        Particles& getParticles() { return m_particles; }
        void addParticle( std::shared_ptr<Particle> particle ) { m_particles.push_back( particle ); }

        Particles& getTempAshFallParticles() { return m_tempAshFallParticles; }
        void addTempAshFallParticles( std::shared_ptr<Particle> particle) { m_tempAshFallParticles.push_back(particle); }

        Blobs& getBlobs() { return m_blobs; }
        void addBlob( std::shared_ptr<Blob> blob );
        void destroyCorruptedBlobs(std::shared_ptr<Particle> corruptedParticle);

        void mergeBlobParticles( std::shared_ptr<Particle> selectedParticle );
        void unmergeBlobParticles( std::shared_ptr<Particle> selectedParticle, float childrenRadius = 2.5f );

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
        void moveCamera(Vector3 moveDirection) { moveDirection.z *= -1;  m_camera.move(moveDirection.v3()); }
        std::pair<glm::vec3, glm::vec3> getCameraInfo() const;

        void draw();
        void drawGround() const;
};

#endif