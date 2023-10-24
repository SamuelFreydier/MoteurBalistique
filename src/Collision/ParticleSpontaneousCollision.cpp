#include "ParticleSpontaneousCollision.h"

/**
 * @brief Génère la collision lorsque deux particules rentrent en contact
 * @param contact 
 * @param limit 
 * @return 0 si pas de collision. 1 sinon.
*/
int ParticleSpontaneousCollision::addContact( ParticleContact* contact, const int& limit ) const
{
    contact->m_particles[ 0 ] = m_particles[ 0 ];
    contact->m_particles[ 1 ] = m_particles[ 1 ];

    // Vecteur entre les deux particules
    Vector3 midline = m_particles[ 0 ]->getPosition() - m_particles[ 1 ]->getPosition();
    float midlineSize = midline.norm();

    // Y a-t-il collision ?
    if( midlineSize <= 0.0f || midlineSize >= m_particles[ 0 ]->getRadius() + m_particles[ 1 ]->getRadius() )
    {
        // Non => distance plus grande que la somme des deux radius
        return 0;
    }

    // Calcul du vecteur normal
    contact->m_contactNormal = -midline.normalized();

    contact->m_penetration = m_particles[ 0 ]->getRadius() + m_particles[ 1 ]->getRadius() - midlineSize;
    contact->m_restitution = m_restitution;

    return 1;
}
