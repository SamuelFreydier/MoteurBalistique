#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
private:
    float m_massReverse;

    Vector m_velocity;
    Vector m_acceleration;
    Vector m_position;

public:
    Particle( const float& mass, const Vector& velocity, const Vector& acceleration, const Vector& position );

    const float& getMassReverse() const { return m_massReverse; }
    void setMassReverse( const float& mass ) { m_massReverse = 1 / mass; }

    const Vector& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector& velocity ) { m_acceleration = velocity; }

    const Vector& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector& acceleration ) { m_acceleration = acceleration; }

    const Vector& getPosition() const { return m_position; }
    void setPosition( const Vector& position ) { m_position = position; }

    void update( const float& time );
    void draw() const;
};

#endif
