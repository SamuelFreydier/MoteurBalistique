#include "ParticleFriction.h"


ParticleFriction::ParticleFriction( const float& frictionCoefficient, const Vector3& gravity )
    : m_frictionCoefficient( frictionCoefficient )
{
    // Que faire avec la gravit� ? => Trouver gn ("r�action � la gravit�" dans le cours) et gk 
    // Comment trouver gn et gk ? Ce sera pour une autre fois => jingle de fin de Papy Grenier
    // Au lieu de passer la gravit�, passer directement la force normale ?
}

void ParticleFriction::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    
}
