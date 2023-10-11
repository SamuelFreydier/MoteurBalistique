#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "Particle.h"

// Interface
class ParticleForceGenerator
{
public:
    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) = 0;
};

#endif