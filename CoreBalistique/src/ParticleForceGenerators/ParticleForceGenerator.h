#ifndef PARTICLEFORCEGENERATOR_H
#define PARTICLEFORCEGENERATOR_H

#include "Particle.h"
#include "Rigidbody.h"

// Interface
class ForceGenerator
{
public:
    virtual void draw(std::shared_ptr<Rigidbody> rigidbody) const {}

    virtual void updateForce( std::shared_ptr<Particle> particle, float secondsElapsedSincePreviousUpdate) = 0;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float secondsElapsedSincePreviousUpdate) = 0;
};

#endif