#include "ParticleSpring.h"

/**
 * @brief Constructeur à partir d'une autre particule, de la constante d'élasticité et de la longueur au repos du ressort
 * @param other 
 * @param springConstant 
 * @param restLength 
*/
Spring::Spring( std::shared_ptr<Particle> other, const float& springConstant, const float& restLength )
    : m_otherParticle( other ), m_springConstant( springConstant ), m_restLength( restLength )
{
}

/**
 * @brief Constructeur à partir d'un autre rigidbody, de la constante d'élasticité et de la longueur au repos du ressort
 * @param other
 * @param springConstant
 * @param restLength
*/
Spring::Spring(std::shared_ptr<Rigidbody> other, const Vector3& localization, const Vector3& otherLocalization, const float& springConstant, const float& restLength)
    : m_otherRigidbody(other), m_localization(localization), m_otherLocalization(otherLocalization), m_springConstant(springConstant), m_restLength(restLength)
{
}

/**
 * @brief Applique la force de ressort à particle. Formule : f = -k (l - l0)
 * @param particle 
 * @param duration 
*/
void Spring::updateForce( std::shared_ptr<Particle> particle, float duration )
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

/**
 * @brief Applique la force de ressort à rigidbody. Formule : f = -k (l - l0)
 * @param particle
 * @param duration
*/
void Spring::updateForce(std::shared_ptr<Rigidbody> rigidbody, float duration)
{
    // Vecteur du ressort
    Vector3 worldLoc = rigidbody->getPointInWorldSpace(m_localization);
    Vector3 worldOtherLoc = m_otherRigidbody->getPointInWorldSpace(m_otherLocalization);
    Vector3 force = worldLoc - worldOtherLoc;

    // Norme de la force
    float norm = force.norm(); // norm = l
    norm = fabs(norm - m_restLength); // norm = l - l0
    norm *= m_springConstant; // norm = k * (l - l0)

    // Calcul de la force finale
    force.normalize(); // Direction du vecteur force
    force *= -norm; // force = - k * (l - l0) dans la bonne direction

    // Ajout de la force
    rigidbody->addForceAtPoint(force, worldLoc);
}
