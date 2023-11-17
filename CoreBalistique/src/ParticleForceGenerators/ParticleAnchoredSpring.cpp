#include "ParticleSpring.h"
#include "ParticleAnchoredSpring.h"

/**
 * @brief Constructeur à partir d'un point fixe de l'espace, de la constante d'élasticité et de la longueur au repos du ressort
 * @param anchor 
 * @param springConstant 
 * @param restLength 
*/
ParticleAnchoredSpring::ParticleAnchoredSpring( Vector3* anchor, const float& springConstant, const float& restLength )
    : m_anchor( anchor ), m_springConstant( springConstant ), m_restLength( restLength )
{
}


/**
 * @brief Applique la force de ressort à particle. Formule : f = -k (l - l0)
 * @param particle
 * @param duration
*/
void ParticleAnchoredSpring::updateForce( std::shared_ptr<Particle> particle, float duration )
{
    // Vecteur du ressort
    Vector3 force( particle->getPosition() );
    force -= *m_anchor;

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

/**
 * @brief Applique la force de ressort à rigidbody. Formule : f = -k (l - l0)
 * @param particle
 * @param duration
*/
void ParticleAnchoredSpring::updateForce(std::shared_ptr<Rigidbody> rigidbody, float duration)
{
    // Vecteur du ressort
    Vector3 force(rigidbody->getPosition());
    force -= *m_anchor;

    // Norme de la force
    float norm = force.norm(); // norm = l
    norm = fabs(norm - m_restLength); // norm = l - l0
    norm *= m_springConstant; // norm = k * (l - l0)

    // Calcul de la force finale
    force.normalize(); // Direction du vecteur force
    force *= -norm; // force = - k * (l - l0) dans la bonne direction

    // Ajout de la force
    rigidbody->addForce(force);
}