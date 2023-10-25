#ifndef PARTICLEAIRFRICTION_H
#define PARTICLEAIRFRICTION_H

#include "ParticleForceGenerator.h"

// Frottements de l'air r�alistes, les frottements de l'air pas r�alistes n'�tant pas vraiment une force, il sont directement trait�s dans Particle.update()
class ParticleAirFriction : public ParticleForceGenerator
{
    private:
        Vector3 m_worldWind; // V�locit� de la masse d'air 

    public:
        ParticleAirFriction(const Vector3& worldWind = Vector3()); 

        virtual void updateForce( std::shared_ptr<Particle> particle, float secondsElapsedSincePreviousUpdate) override;

        const Vector3& getWorldWind() const { return m_worldWind; }
        void setWorldWind(const Vector3& worldWind) { m_worldWind = worldWind; }
};

#endif