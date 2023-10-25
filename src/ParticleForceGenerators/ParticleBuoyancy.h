#ifndef PARTICLEBUOYANCY_H
#define PARTICLEBUOYANCY_H

#include "ParticleForceGenerator.h"

// Applique une force de flottabilit� par rapport � un plan liquide sur le plan XZ
class ParticleBuoyancy : public ParticleForceGenerator
{
private:
    // Profondeur maximale permise avant de g�n�rer la flottabilit� maximale
    float m_maxDepth;

    // Volume de l'objet
    float m_volume;

    // Hauteur du plan d'eau (inf�rieur � y = hauteur de la fen�tre)
    float m_waterHeight;

    // Densit� du liquide => Eau = 1000 kg/m�tre cube
    float m_liquidDensity;

public:
    ParticleBuoyancy( const float& maxDepth, const float& volume, const float& waterHeight, const float& liquidDensity = 1000.0f );

    virtual void updateForce( std::shared_ptr<Particle> particle, float duration ) override;
};

#endif