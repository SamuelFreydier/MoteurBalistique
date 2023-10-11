#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort uniquement lorsque le ressort s'étend (saut à l'élastique)
class ParticleBungee : public ParticleForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au point à partir duquel il commence à générer une force
    float m_restLength;

public:
    ParticleBungee( const std::shared_ptr<Particle>& otherParticle, const float& springConstant, const float& restLength );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif