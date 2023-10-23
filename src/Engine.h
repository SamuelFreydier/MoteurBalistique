#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleForceGenerators/ParticleForceRegistry.h"
#include "ParticleForceGenerators/ParticleGravity.h"
#include "Fireball.h"
#include "Referential.h"


using ParticlePtr = std::shared_ptr<Particle>;

class Engine
{
    private:
        // Singleton
        static Engine* s_engine;

        // Referentiel cartésien
        static Referential s_referential;

        std::list<ParticlePtr> m_particles;
    
        // Registre (associations particule / générateur de force) => Gère gravité/frottements/ressorts
        ParticleForceRegistry m_particleForceRegistry;

        // Gravité mise à jour selon les paramètres utilisateur
        Vector3 m_gravity;

        // Frottements pas réaliste
        static float s_damping;

        // Frottement air réaliste
        static bool s_realisticAirResistance;

        // Variation des couleurs
        static int s_colorShift;

        // score : +1 par boule de feu explosée !!
        int score = 0;

    protected:
        Engine() = default;

    public:
        Engine( Engine& ) = delete;
        void operator=( const Engine& ) = delete;

        static Engine* getInstance();

        static Referential& getReferential() { return s_referential; };

        const std::list<ParticlePtr>& getParticles() const { return m_particles; }
        void addParticle( ParticlePtr particle ) { m_particles.push_back( particle ); }
        void deleteParticle( ParticlePtr particle ) { m_particles.remove( particle ); }

    const Vector3& getGravity() const { return m_gravity; }
    void setGravity( const Vector3& gravity ) { m_gravity = gravity; }

        static const float& getDamping() { return s_damping; }
        static void setDamping( const float& damping ) { s_damping = damping; }

        static const bool& getRealisticAirResistance() { return s_realisticAirResistance; }
        static void setRealisticAirResistance(const bool& newRealisticAirResistance) { s_realisticAirResistance = newRealisticAirResistance; }

        static const int& getColorShift() { return s_colorShift; }
        static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

        static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
        static Vector3 randshiftColor( const Vector3& color, const int& shiftAmount );

        // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
        void shootParticle( const Vector3& initialPos, const Vector3& initialVelocity, const float& mass = 1.0, const float& radius = 1.0, const Vector3& color = Vector3( { 255, 0, 0 } ), bool isFireball = false, bool m_showParticleInfos = false);
   
        // Mise à jour PHYSIQUE des particules
        void update( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Mise à jour GRAPHIQUE des particules
        void drawParticles() const;

        // Déclenche une action si une particule est présente à l'endroit d'un clic souris. Renvoie false si rien n'a été cliqué.
        bool clickedParticle( const float& x, const float& y );

        const int& getScore() const { return score; };
        void increaseScore(const int& points = 1) { score += points; };
        void showScore(const bool& boolShowScore) const;
};

#endif