#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "Particle.h"

// Interface
class ParticleForceGenerator
{
public:
    virtual void updateForce( const std::shared_ptr<Particle>& particle, const float& secondsElapsedSincePreviousUpdate) = 0;
};

#endif