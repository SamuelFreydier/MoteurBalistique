#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort
class ParticleSpring : public ParticleForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    ParticleSpring( const std::shared_ptr<Particle>& other, const float& springConstant, const float& restLength );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif