#include "ParticleCable.h"

/**
* @brief Génère les collisions pour éviter que le câble soit brisé.
* @param contact
* @param limit (>= 1)
* @return 0 si le câble n'est pas sur-étendu. 1 si une collision est nécessaire.
*/
int ParticleCable::addContact( std::shared_ptr<ParticleContact> contact, const int& limit ) const
{
    // Longueur du câble
    float length = currentLength();

    // Est-ce que le câble est trop étendu ?
    if( length < m_maxLength )
    {
        // Si non => pas besoin de collision
        return 0;
    }

    // Câble trop étendu => Création de collision
    contact->m_particles[ 0 ] = m_particles[ 0 ];
    contact->m_particles[ 1 ] = m_particles[ 1 ];

    // Calcul du vecteur normal
    Vector3 normal = m_particles[ 1 ]->getPosition() - m_particles[ 0 ]->getPosition();
    normal.normalize();
    contact->m_contactNormal = normal;
    contact->m_penetration = length - m_maxLength;
    contact->m_restitution = m_restitution;

    return 1;
}
