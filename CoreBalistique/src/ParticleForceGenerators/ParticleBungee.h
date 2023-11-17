#ifndef PARTICLEBUNGEE_H
#define PARTICLEBUNGEE_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort uniquement lorsque le ressort s'étend (saut à l'élastique)
class ParticleBungee : public ForceGenerator
{
private:
    // Particule à l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    //Point d'application du ressort sur ce rigidbody (coordonnées monde)
    Vector3 m_localization;

    //Point d'application du ressort sur le rigidbody à l'autre bout du ressort (coordonnées monde)
    Vector3 m_otherLocalization;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au point à partir duquel il commence à générer une force
    float m_restLength;

public:
    ParticleBungee( std::shared_ptr<Particle> otherParticle, const float& springConstant, const float& restLength);
    ParticleBungee(const Vector3& localization, const Vector3& otherLocalization, const float& springConstant, const float& restLength);

    const float& getSpringConstant() const { return m_springConstant; }

    const float& getRestLength() const { return m_restLength; }

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;
};

#endif