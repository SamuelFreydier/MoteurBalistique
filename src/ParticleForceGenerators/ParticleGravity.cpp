#include "ParticleGravity.h"

ParticleGravity::ParticleGravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void ParticleGravity::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    if( particle->getMass() > 0.f )
    {
        particle->addForce( m_gravity * particle->getMass() );
    }
}
