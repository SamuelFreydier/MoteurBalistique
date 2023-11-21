#ifndef PARTICLEFORCEREGISTRY_H
#define PARTICLEFORCEREGISTRY_H

#include "ParticleForceGenerator.h"

class ForceRegistry
{
public:
    struct ParticleForceRegistration
    {
        std::shared_ptr<Particle> particle;
        std::shared_ptr<ForceGenerator> forceGenerator;
    };

    struct RigidbodyForceRegistration
    {
        std::shared_ptr<Rigidbody> rigidbody;
        std::shared_ptr<ForceGenerator> forceGenerator;
    };

    typedef std::vector<ParticleForceRegistration> ParticleRegistry;
    typedef std::vector<RigidbodyForceRegistration> RigidbodyRegistry;
 
private:
    ParticleRegistry m_particleRegistry;
    RigidbodyRegistry m_rigidbodyRegistry;

public:
    //particules
    void add( std::shared_ptr<Particle> particle, std::shared_ptr<ForceGenerator> forceGenerator );
    void remove( std::shared_ptr<Particle> particle, std::shared_ptr<ForceGenerator> forceGenerator );
    void remove( std::shared_ptr<Particle> particle );

    //rigidbodies
    void add(std::shared_ptr<Rigidbody> rigidbody, std::shared_ptr<ForceGenerator> forceGenerator);
    void remove(std::shared_ptr<Rigidbody> rigidbody, std::shared_ptr<ForceGenerator> forceGenerator);
    void remove(std::shared_ptr<Rigidbody> rigidbody);

    void clear();
    void updateForces( const float& secondsElapsedSincePreviousUpdate);
    void drawForces();
};

#endif