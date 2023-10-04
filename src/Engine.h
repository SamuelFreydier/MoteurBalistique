#ifndef ENGINE_H
#define ENGINE_H

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
        // Particules qui ne font rien de sp�cial (pas de r�action au clic) => typiquement les train�es des boules de feu
        std::list<ParticlePtr> m_vanillaParticles;
    
        // Gravit�
        static Vector s_gravity;

        // Frottement
        static float s_damping;

        // Variation des couleurs
        static int s_colorShift;


        // � interpreter comme un nombre binaire sur 3 bits, bits 1 = clic gauche, bits 2 = clic scroll, bits 3 = clic droit donc les valeurs possibles sont comprises entre 0 et 7
        static int s_mouseButtonPressed;

    protected:
        Engine() = default;

    public:
        Engine( Engine& ) = delete;
        void operator=( const Engine& ) = delete;

        static Engine* getInstance();

        static Referential& getReferential() { return s_referential; }

        const std::list<ParticlePtr>& getParticles() const { return m_particles; }
        void addParticle( ParticlePtr particle ) { m_particles.push_back( particle ); }
        void deleteParticle( ParticlePtr particle ) { m_particles.remove( particle ); }

        const std::list<ParticlePtr>& getVanillaParticles() const { return m_vanillaParticles; }
        void addVanillaParticle( ParticlePtr particle ) { m_vanillaParticles.push_back( particle ); }
        void deleteVanillaParticle( ParticlePtr particle ) { m_vanillaParticles.remove( particle ); }
    
        static const Vector& getGravity() { return s_gravity; }
        static void setGravity( const Vector& gravity ) { s_gravity = gravity; }

        static const float& getDamping() { return s_damping; }
        static void setDamping( const float& damping ) { s_damping = damping; }

        static const int& getColorShift() { return s_colorShift; }
        static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

        static const int& getMouseButtonPressed() { return s_mouseButtonPressed; }
        static void setMouseButtonPressed(const int& valeurPlusOuMoins) { s_mouseButtonPressed += valeurPlusOuMoins; }

        static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
        static Vector randshiftColor( const Vector& color, const int& shiftAmount );

        // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
        void shootParticle( const Vector& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass = 1.0, const float& radius = 1.0, const Vector& color = Vector( { 255, 0, 0 } ), bool isFireball = false );
   
        // Mise � jour PHYSIQUE des particules
        void update( const float& deltaTime );
        void updateParticleList( std::list<ParticlePtr>& particleList, const float& deltaTime );

        // Mise � jour GRAPHIQUE des particules
        void drawParticles() const;

        // D�clenche une action si une particule est pr�sente � l'endroit d'un clic souris. Renvoie false si rien n'a �t� cliqu�.
        bool clickedParticle( const float& x, const float& y );
};

#endif