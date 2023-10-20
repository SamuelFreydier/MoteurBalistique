#include "ParticleSpring.h"

/**
 * @brief Constructeur � partir d'une autre particule, de la constante d'�lasticit� et de la longueur au repos du ressort
 * @param other 
 * @param springConstant 
 * @param restLength 
*/
ParticleSpring::ParticleSpring( Particle* other, const float& springConstant, const float& restLength )
    : m_otherParticle( other ), m_springConstant( springConstant ), m_restLength( restLength )
{
}

/**
 * @brief Applique la force de ressort � particle. Formule : f = -k (l - l0)
 * @param particle 
 * @param duration 
*/
void ParticleSpring::updateForce( Particle* particle, float duration )
{
    // Vecteur du ressort
    Vector3 force( particle->getPosition() );
    force -= m_otherParticle->getPosition();

    // Norme de la force
    float norm = force.norm(); // norm = l
    norm = fabs( norm - m_restLength ); // norm = l - l0
    norm *= m_springConstant; // norm = k * (l - l0)

    // Calcul de la force finale
    force.normalize(); // Direction du vecteur force
    force *= -norm; // force = - k * (l - l0) dans la bonne direction

    // Ajout de la force
    particle->addForce( force );
}
