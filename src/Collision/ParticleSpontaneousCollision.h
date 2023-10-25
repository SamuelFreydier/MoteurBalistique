#ifndef PARTICLESPONTANEOUSCOLLISION_H
#define PARTICLESPONTANEOUSCOLLISION_H

#include "ParticleContactGenerator.h"

/**
 * @brief Détecte les collisions spontanées entre des particules (= pas de comportement particulier comme un générateur)
*/
class ParticleSpontaneousCollision : public ParticleContactGenerator
{
public:
    // Paire de particules entrechoquées
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Coefficient de restitution du choc (rebond)
    float m_restitution;
public:
    virtual int addContact( ParticleContact* contact, const int& limit ) const override;

};

#endif