#include "ParticleGravity.h"

Gravity::Gravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void Gravity::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    float particleMass = particle->getMass();

    if(particleMass > 0.f )
    {
        particle->addForce( m_gravity * particleMass);
    }
}

void Gravity::updateForce(std::shared_ptr<Rigidbody> rigidbody, float duration)
{
    float rigidbodyMass = rigidbody->getMass();

    if (rigidbodyMass > 0.f)
    {
        rigidbody->addForce(m_gravity * rigidbodyMass);
    }
}