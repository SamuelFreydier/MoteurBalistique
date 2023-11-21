#ifndef PARTICLESPRING_H
#define PARTICLESPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort
class Spring : public ForceGenerator
{
private:
    // Particule � l'autre bout du ressort
    std::shared_ptr<Particle> m_otherParticle;

    std::shared_ptr<Rigidbody> m_otherRigidbody;

    //Point d'application du ressort sur ce rigidbody (coordonn�es dans le r�f�rentiel de l'objet)
    Vector3 m_localization;

    //Point d'application du ressort sur le rigidbody � l'autre bout du ressort (coordonn�es dans le r�f�rentiel de l'objet)
    Vector3 m_otherLocalization;

    // Constante d'�lasticit�
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    Spring( std::shared_ptr<Particle> other, const float& springConstant, const float& restLength );
    Spring( std::shared_ptr<Rigidbody> other, const Vector3& localization, const Vector3& otherLocalization, const float& springConstant, const float& restLength );

    const float& getSpringConstant() const { return m_springConstant; }

    const float& getRestLength() const { return m_restLength; }

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;
};

#endif