#include "ParticleGravity.h"

ParticleGravity::ParticleGravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void ParticleGravity::updateForce( Particle* particle, float duration )
{
    float particleMass = particle->getMass();

    if(particleMass > 0.f )
    {
        particle->addForce( m_gravity * particleMass);
    }
}
