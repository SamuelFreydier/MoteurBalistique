#ifndef PARTICLEANCHOREDSPRING_H
#define PARTICLEANCHOREDSPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort, avec une extr�mit� fix�e � un point de l'espace
class ParticleAnchoredSpring : public ForceGenerator
{
private:
    // Ancre
    Vector3* m_anchor;

    //Point d'application du ressort sur ce rigidbody (coordonn�es dans le r�f�rentiel de l'objet)
    Vector3 m_localization;

    // Constante d'�lasticit�
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    ParticleAnchoredSpring( Vector3* anchor, const Vector3& localization, const float& springConstant, const float& restLength );

    const float& getSpringConstant() const { return m_springConstant; }

    const float& getRestLength() const { return m_restLength; }

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
    virtual void updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration ) override;
};

#endif