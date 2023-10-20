#ifndef PARTICLEFORCEREGISTRY_H
#define PARTICLEFORCEREGISTRY_H

#include "ParticleForceGenerator.h"

class ParticleForceRegistry
{
public:
    struct ParticleForceRegistration
    {
        Particle* particle;
        ParticleForceGenerator* forceGenerator;
    };

    typedef std::vector<ParticleForceRegistration> Registry;
 
private:
    Registry m_registry;

public:
    void add( Particle* particle, ParticleForceGenerator* forceGenerator );
    void remove( Particle* particle, ParticleForceGenerator* forceGenerator );
    void remove( Particle* particle );
    void clear();
    void updateForces( const float& duration );
};

#endif