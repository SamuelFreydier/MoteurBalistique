#ifndef ENGINE_H
#define ENGINE_H

#include "Fireball.h"

using ParticlePtr = std::shared_ptr<Particle>;

class Engine
{
private:
    // Singleton
    static Engine* s_engine;

    std::list<ParticlePtr> m_particles;
    // Particules qui ne font rien de spécial (pas de réaction au clic) => typiquement les trainées des boules de feu
    std::list<ParticlePtr> m_vanillaParticles;
    
    // Gravité
    static Vector s_gravity;

    // Frottement
    static float s_damping;

    // Variation des couleurs
    static int s_colorShift;
protected:
    Engine() = default;
public:
    Engine( Engine& ) = delete;
    void operator=( const Engine& ) = delete;

    static Engine* getInstance();

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

    static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
    static Vector randshiftColor( const Vector& color, const int& shiftAmount );

    // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
    void shootParticle( const Vector& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass = 1.0, const float& radius = 1.0, const Vector& color = Vector( { 255, 0, 0 } ), bool isFireball = false );
   
    // Mise à jour PHYSIQUE des particules
    void update( const float& deltaTime );
    void updateParticleList( std::list<ParticlePtr>& particleList, const float& deltaTime );

    // Mise à jour GRAPHIQUE des particules
    void drawParticles() const;

    // Déclenche une action si une particule est présente à l'endroit d'un clic souris. Renvoie false si rien n'a été cliqué.
    bool clickedParticle( const float& x, const float& y );
};

#endif