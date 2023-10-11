#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort uniquement lorsque le ressort s'�tend (saut � l'�lastique)
class ParticleBungee : public ParticleForceGenerator
{
private:
    // Particule � l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    // Constante d'�lasticit�
    float m_springConstant;

    // Longueur du ressort au point � partir duquel il commence � g�n�rer une force
    float m_restLength;

public:
    ParticleBungee( const std::shared_ptr<Particle>& otherParticle, const float& springConstant, const float& restLength );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif