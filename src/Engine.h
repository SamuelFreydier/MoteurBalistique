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

        // Referentiel cart�sien
        static Referential s_referential;

        std::list<ParticlePtr> m_particles;
    
        // Registre (associations particule / g�n�rateur de force) => G�re gravit�/frottements/ressorts
        ParticleForceRegistry m_particleForceRegistry;

        // Gravit� mise � jour selon les param�tres utilisateur
        Vector3 m_gravity;

        // Frottements pas r�aliste
        static float s_damping;

        // Frottement air r�aliste
        static bool s_realisticAirResistance;

        // Variation des couleurs
        static int s_colorShift;

        // score : +1 par boule de feu explos�e !!
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
   
        // Mise � jour PHYSIQUE des particules
        void update( const float& secondsElapsedSincePreviousUpdate);

        // Nettoyage des particules non visibles
        void cleanup();

        // Mise � jour GRAPHIQUE des particules
        void drawParticles() const;

        // D�clenche une action si une particule est pr�sente � l'endroit d'un clic souris. Renvoie false si rien n'a �t� cliqu�.
        bool clickedParticle( const float& x, const float& y );

        const int& getScore() const { return score; };
        void increaseScore(const int& points = 1) { score += points; };
        void showScore(const bool& boolShowScore) const;
};

#endif