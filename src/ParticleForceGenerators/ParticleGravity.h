#ifndef PARTICLEGRAVITY_H
#define PARTICLEGRAVITY_H

#include "ParticleForceGenerator.h"

// G�n�rateur de gravit�
class ParticleGravity : public ParticleForceGenerator
{
private:
    Vector m_gravity;

public:
    ParticleGravity( const Vector& gravity = Vector( { 0, 9.81, 0 } ) );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;

    const Vector& getGravity() const { return m_gravity; }
    void setGravity( const Vector& gravity ) { m_gravity = gravity; }
};

#endif