#ifndef ENGINE_H
#define ENGINE_H

#include "Particle.h"

class Engine
{
private:
    // Singleton
    static Engine* s_engine;

    std::vector<Particle> m_particles;
    static Vector s_gravity;
    static float s_damping;
protected:
    Engine() = default;
public:
    Engine( Engine& ) = delete;
    void operator=( const Engine& ) = delete;

    static Engine* getInstance();

    const std::vector<Particle>& getParticles() const { return m_particles; }
    
    static const Vector& getGravity() { return s_gravity; }
    static void setGravity( const Vector& gravity ) { s_gravity = gravity; }

    static const float& getDamping() { return s_damping; }
    static void setDamping( const float& damping ) { s_damping = damping; }

    void shootParticle( const float& initialAngle, const float& initialSpeed, const float& mass = 1.0, bool hasTrail = false );
    void update( const float& time );
    void drawParticles() const;
};

#endif