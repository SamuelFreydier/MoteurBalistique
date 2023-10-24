#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "Particle.h"

// Interface
class ParticleForceGenerator
{
public:
    virtual void updateForce( Particle* particle, float secondsElapsedSincePreviousUpdate) = 0;
};

#endif