#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
    private:
    float m_massReverse;
    float m_velocity;
    float m_acceleration;
    Vector m_position;

    public:
    Particle( float mass, float velocity, float acceleration, Vector position );

    const float GetMassReverse() const { return m_massReverse; }
    void SetMassReverse( const float& mass ) { m_massReverse = 1 / mass; }

    const float GetVelocity() const { return m_velocity; }
    void SetVelocity( const float& velocity ) { m_acceleration = velocity; }

    const float GetAcceleration() const { return m_acceleration; }
    void SetAcceleration( const float& acceleration ) { m_acceleration = acceleration; }

    const Vector GetPosition() const { return m_position; }
    void SetPosition( const Vector& position ) { m_position = position; }
};

#endif
