#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort uniquement lorsque le ressort s'�tend (saut � l'�lastique)
class ParticleBungee : public ForceGenerator
{
private:
    // Particule � l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;
    std::shared_ptr<Rigidbody> m_otherRigidbody;

    // Constante d'�lasticit�
    float m_springConstant;

    // Longueur du ressort au point � partir duquel il commence � g�n�rer une force
    float m_restLength;

public:
    ParticleBungee( std::shared_ptr<Particle> otherParticle, const float& springConstant, const float& restLength);
    ParticleBungee( std::shared_ptr<Rigidbody> otherRigidbody, const float& springConstant, const float& restLength);

    const float& getSpringConstant() const { return m_springConstant; }

    const float& getRestLength() const { return m_restLength; }

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;
};

#endif