#ifndef PARTICLEANCHOREDSPRING_H
#define PARTICLEANCHOREDSPRING_H

#include "ParticleForceGenerator.h"

// Applique une force de ressort, avec une extrémité fixée à un point de l'espace
class ParticleAnchoredSpring : public ParticleForceGenerator
{
private:
    // Ancre
    Vector3* m_anchor;

    // Constante d'élasticité
    float m_springConstant;

    // Longueur du ressort au repos
    float m_restLength;

public:
    ParticleAnchoredSpring( Vector3* anchor, const float& springConstant, const float& restLength );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif