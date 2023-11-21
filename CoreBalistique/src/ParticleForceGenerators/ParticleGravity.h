#ifndef PARTICLEGRAVITY_H
#define PARTICLEGRAVITY_H

#include "ParticleForceGenerator.h"

// Générateur de gravité
class Gravity : public ForceGenerator
{
private:
    Vector3 m_gravity;

public:
    Gravity( const Vector3& gravity = Vector3( 0, -9.81, 0 ) );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;

    const Vector3& getGravity() const { return m_gravity; }
    void setGravity( const Vector3& gravity ) { m_gravity = gravity; }
};

#endif