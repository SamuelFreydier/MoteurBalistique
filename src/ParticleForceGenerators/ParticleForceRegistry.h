#ifndef PARTICLEFORCEREGISTRY_H
#define PARTICLEFORCEREGISTRY_H

#include "ParticleForceGenerator.h"

class ParticleForceRegistry
{
public:
    struct ParticleForceRegistration
    {
        std::shared_ptr<Particle> particle;
        std::shared_ptr<ParticleForceGenerator> forceGenerator;
    };

    typedef std::vector<ParticleForceRegistration> Registry;
 
private:
    Registry m_registry;

public:
    void add( std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator );
    void remove( std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator );
    void remove( std::shared_ptr<Particle> particle );
    void clear();
    void updateForces( const float& duration );
};

#endif