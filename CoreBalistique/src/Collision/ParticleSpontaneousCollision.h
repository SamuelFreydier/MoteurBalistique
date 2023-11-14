#ifndef PARTICLESPONTANEOUSCOLLISION_H
#define PARTICLESPONTANEOUSCOLLISION_H

#include "ParticleContactGenerator.h"

/**
 * @brief D�tecte les collisions spontan�es entre des particules (= pas de comportement particulier comme un g�n�rateur)
*/
class ParticleSpontaneousCollision : public ParticleContactGenerator
{
public:
    // Paire de particules entrechoqu�es
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Coefficient de restitution du choc (rebond)
    float m_restitution;
public:
    virtual int addContact( ParticleContact* contact, const int& limit ) const override;

};

#endif