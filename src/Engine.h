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
protected:
    Engine() = default;
public:
    Engine( Engine& ) = delete;
    void operator=( const Engine& ) = delete;

    static Engine* getInstance();

    const std::vector<Particle>& getParticles() const { return m_particles; }
    static const Vector& getGravity() { return s_gravity; }

    void shootParticle( const float& initialAngle, const float& initialSpeed );
    void update( const float& time );
    void drawParticles() const;
};

#endif