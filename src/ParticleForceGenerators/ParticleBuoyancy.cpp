#include "ParticleBuoyancy.h"

/**
 * @brief Constructeur � partir d'une profondeur maximale, d'un volume, de la hauteur du plan d'eau, et de la densit� du liquide
 * @param other
 * @param springConstant
 * @param restLength
*/
ParticleBuoyancy::ParticleBuoyancy( const float& maxDepth, const float& volume, const float& waterHeight, const float& liquidDensity )
    : m_maxDepth( maxDepth ), m_volume( volume ), m_waterHeight( waterHeight ), m_liquidDensity( liquidDensity )
{
}

/**
 * @brief Applique la force de flottabilit� � particle
 * @param particle 
 * @param duration 
*/
void ParticleBuoyancy::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    // Calcul de la profondeur de la particule dans le liquide
    float depth = particle->getPosition().getY();

    // Hors de l'eau ? Attention pour rappel le y est espi�gle car il cro�t vers le bas
    if( depth <= m_waterHeight - m_maxDepth )
    {
        // Si oui => Pas de force appliqu�e
        return;
    }

    Vector3 force;

    // Force maximale en cas de profondeur maximale atteinte
    force.y = m_liquidDensity * m_volume;

    // Profondeur maximale non atteinte (particule partiellement submerg�e)
    if( depth < m_waterHeight + m_maxDepth )
    {
        // Coefficient att�nuant
        //float submergedProportion = ( depth - m_maxDepth - m_waterHeight ) / (2 * m_maxDepth); // Entre 0 et 1        force.y = m_liquidDensity * m_volume * submergedProportion;
        //force.y *= submergedProportion;
    }

    particle->addForce( force );
}
