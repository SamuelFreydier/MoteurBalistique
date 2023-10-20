#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort
class ParticleSpring : public ParticleForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    Particle* m_otherParticle;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    ParticleSpring( Particle* other, const float& springConstant, const float& restLength );

    virtual void updateForce( Particle* particle, float duration ) override;
};

#endif