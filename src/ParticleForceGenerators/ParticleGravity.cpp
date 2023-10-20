#include "ParticleGravity.h"

ParticleGravity::ParticleGravity( const Vector3& gravity )
    : m_gravity( gravity )
{

}

void ParticleGravity::updateForce( Particle* particle, float duration )
{
    if( particle->getMass() > 0.f )
    {
        particle->addForce( m_gravity * particle->getMass() );
    }
}
