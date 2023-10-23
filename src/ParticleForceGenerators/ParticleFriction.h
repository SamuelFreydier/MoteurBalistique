#ifndef PARTICLEFRICTION_H
#define PARTICLEFRICTION_H

#include "ParticleForceGenerator.h"
#include "Engine.h"

// G�n�rateur de friction (statique ou cin�tique)
class ParticleFriction : public ParticleForceGenerator
{
private:
    Vector3 m_friction;
    float m_frictionCoefficient;
    bool m_isRealisticAirFrictionForce = false;

public:
    ParticleFriction( const float& frictionCoefficient, const Vector3& gravity = Vector3( 0, -9.81, 0 ) ); 
    ParticleFriction(const bool& isRealisticAirFrictionForce); // Frottements de l'air r�alistes, les frottements de l'air pas r�alistes n'�tant pas vraiment une force, il sont directement trait�s dans Particle.update()

    virtual void updateForce( const std::shared_ptr<Particle>& particle, const float& secondsElapsedSincePreviousUpdate) override;

    const Vector3& getFriction() const { return m_friction; }
    void setFriction( const Vector3& friction ) { m_friction = friction; }

    const float& getFrictionCoefficient() const { return m_frictionCoefficient; }
    void setFrictionCoefficient( const float& frictionCoefficient ) { m_frictionCoefficient = frictionCoefficient; }
};

#endif