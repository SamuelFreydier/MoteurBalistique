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
    Vector m_color;

    bool m_hasTrail;

public:
    Particle( const float& mass = 1.0, bool hasTrail = false, const Vector& velocity = Vector( { 0.0, 0.0, 0.0 } ), const Vector& acceleration = Vector( { 0.0, 0.0, 0.0 } ), const Vector& position = Vector( { 0.0, 0.0, 0.0 } ), const Vector& color = Vector( { 255, 0, 0 } ) );
    Particle( const Particle& particle );

    const float& getMassReverse() const { return m_massReverse; }
    const float& getMass() const { return 1 / m_massReverse; }
    void setMassReverse( const float& mass ) { m_massReverse = 1 / mass; }

    const Vector& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector& velocity ) { m_acceleration = velocity; }

    const Vector& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector& acceleration ) { m_acceleration = acceleration; }

    const Vector& getPosition() const { return m_position; }
    void setPosition( const Vector& position ) { m_position = position; }

    const Vector& getColor() const { return m_color; }
    void setColor( const Vector& color ) { m_color = color; }

    bool hasTrail() const { return m_hasTrail; }

    void update( const float& time );
    void draw() const;
};

#endif
