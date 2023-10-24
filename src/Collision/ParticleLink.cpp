#include "ParticleLink.h"

/*
ParticleLink::ParticleLink(Particle* particle1, Particle* particle2)
{
    m_particles[0] = particle1;
    m_particles[1] = particle2;
}*/


float ParticleLink::currentLength() const
{
    //Vector3 relativePos = m_particles[ 0 ]->getPosition() - m_particles[ 1 ]->getPosition();
    return m_particles[0]->getPosition().distance(m_particles[1]->getPosition());
}