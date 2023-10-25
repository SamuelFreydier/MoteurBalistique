#ifndef PARTICLEBUOYANCY_H
#define PARTICLEBUOYANCY_H

#include "ParticleForceGenerator.h"

// Applique une force de flottabilité par rapport à un plan liquide sur le plan XZ
class ParticleBuoyancy : public ParticleForceGenerator
{
private:
    // Profondeur maximale permise avant de générer la flottabilité maximale
    float m_maxDepth;

    // Volume de l'objet
    float m_volume;

    // Hauteur du plan d'eau (inférieur à y = hauteur de la fenêtre)
    float m_waterHeight;

    // Densité du liquide => Eau = 1000 kg/mètre cube
    float m_liquidDensity;

public:
    ParticleBuoyancy( const float& maxDepth, const float& volume, const float& waterHeight, const float& liquidDensity = 1000.0f );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif