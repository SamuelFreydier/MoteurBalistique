#ifndef PARTICLEGRAVITY_H
#define PARTICLEGRAVITY_H

#include "ParticleForceGenerator.h"

// Générateur de gravité
class ParticleGravity : public ParticleForceGenerator
{
private:
    Vector3 m_gravity;

public:
    ParticleGravity( const Vector3& gravity = Vector3( 0, -9.81, 0 ) );

    virtual void updateForce( Particle* particle, float duration ) override;

    const Vector3& getGravity() const { return m_gravity; }
    void setGravity( const Vector3& gravity ) { m_gravity = gravity; }
};

#endif