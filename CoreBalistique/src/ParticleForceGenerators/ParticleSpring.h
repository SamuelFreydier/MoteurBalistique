#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort
class ParticleSpring : public ForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    //Rigidbody à l'autre bout du ressort
    std::shared_ptr<Rigidbody> m_otherRigidbody;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    ParticleSpring( std::shared_ptr<Particle> other, const float& springConstant, const float& restLength );
    ParticleSpring( std::shared_ptr<Rigidbody> other, const float& springConstant, const float& restLength );

    const float& getSpringConstant() const { return m_springConstant; }

    const float& getRestLength() const { return m_restLength; }

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;
};

#endif