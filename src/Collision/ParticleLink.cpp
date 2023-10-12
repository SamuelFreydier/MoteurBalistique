#include "ParticleLink.h"

float ParticleLink::currentLength() const
{
    Vector3 relativePos = m_particles[ 0 ]->getPosition() - m_particles[ 1 ]->getPosition();
    return relativePos.norm();
}