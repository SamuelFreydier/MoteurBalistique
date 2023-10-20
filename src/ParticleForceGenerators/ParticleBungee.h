#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort uniquement lorsque le ressort s'étend (saut à l'élastique)
class ParticleBungee : public ParticleForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    Particle* m_otherParticle;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au point à partir duquel il commence à générer une force
    float m_restLength;

public:
    ParticleBungee( Particle* otherParticle, const float& springConstant, const float& restLength);

    virtual void updateForce( Particle* particle, float duration ) override;
};

#endif