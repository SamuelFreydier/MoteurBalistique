#include "ParticleGravity.h"

ParticleGravity::ParticleGravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void ParticleGravity::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    float particleMass = particle->getMass();

    if(particleMass > 0.f )
    {
        particle->addForce( m_gravity * particleMass);
    }
}
