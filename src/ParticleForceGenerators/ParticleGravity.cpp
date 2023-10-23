#include "ParticleGravity.h"

ParticleGravity::ParticleGravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void ParticleGravity::updateForce( const std::shared_ptr<Particle>& particle, const float& duration )
{
    float particleMass = particle->getMass();

    if(particleMass > 0.f )
    {
        particle->addForce( m_gravity * particleMass);
    }
}
