#include "ParticleBungee.h"

/**
 * @brief Constructeur à partir d'une autre particule, de la constante d'élasticité et de la longueur du ressort à partir de laquelle la force s'applique
 * @param other
 * @param springConstant
 * @param restLength
*/
ParticleBungee::ParticleBungee( std::shared_ptr<Particle> otherParticle, const float& springConstant, const float& restLength )
    : m_otherParticle( otherParticle ), m_springConstant( springConstant ), m_restLength( restLength )
{
}

/**
 * @brief Constructeur à partir d'un autre rigidbody, de la constante d'élasticité et de la longueur du ressort à partir de laquelle la force s'applique
 * @param other
 * @param springConstant
 * @param restLength
*/
ParticleBungee::ParticleBungee(std::shared_ptr<Rigidbody> otherRigidbody, const float& springConstant, const float& restLength)
    : m_otherRigidbody(otherRigidbody), m_springConstant(springConstant), m_restLength(restLength)
{
}


/**
 * @brief Applique la force de ressort à particle. Formule : f = -k (l - l0)
 * @param particle
 * @param duration
*/
void ParticleBungee::updateForce(std::shared_ptr<Particle> particle, float duration)
{
    // Vecteur du ressort
    Vector3 force(particle->getPosition());
    force -= m_otherParticle->getPosition();

    // Longueur entre les deux particules
    float norm = force.norm(); // norm = l

    // Est-ce que le bungee est compressé (= n'atteint pas la longueur nécessaire) ?
    if (norm <= m_restLength)
    {
        // Si oui => aucune force appliquée
        return;
    }

    // Norme de la force
    norm = fabs(norm - m_restLength); // norm = l - l0
    norm *= m_springConstant; // norm = k * (l - l0)

    // Calcul de la force finale
    force.normalize(); // Direction du vecteur force
    force *= -norm; // force = - k * (l - l0) dans la bonne direction

    // Ajout de la force
    particle->addForce(force);
}


/**
 * @brief Applique la force de ressort à rigidbody. Formule : f = -k (l - l0)
 * @param rigidbody
 * @param duration
*/
void ParticleBungee::updateForce( std::shared_ptr<Rigidbody> rigidbody, float duration )
{
    // Vecteur du ressort
    Vector3 force(rigidbody->getPosition() );
    force -= m_otherRigidbody->getPosition();

    // Longueur entre les deux particules
    float norm = force.norm(); // norm = l

    // Est-ce que le bungee est compressé (= n'atteint pas la longueur nécessaire) ?
    if( norm <= m_restLength ) 
    {
        // Si oui => aucune force appliquée
        return;
    }

    // Norme de la force
    norm = fabs( norm - m_restLength ); // norm = l - l0
    norm *= m_springConstant; // norm = k * (l - l0)

    // Calcul de la force finale
    force.normalize(); // Direction du vecteur force
    force *= -norm; // force = - k * (l - l0) dans la bonne direction

    // Ajout de la force
    rigidbody->addForce( force );
}