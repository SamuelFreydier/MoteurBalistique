#include "ParticleRod.h"

/**
* @brief G�n�re les collisions pour �viter que la tige soit rompue.
* @param contact
* @param limit (>= 1)
* @return 0 si la tige n'est pas menac�e par un rapprochement ou un �loignement. 1 si une collision est n�cessaire.
*/
int ParticleRod::addContact( std::shared_ptr<ParticleContact> contact, const int& limit ) const
{
    // Longueur du c�ble
    float currentLen = currentLength();

    // Est-ce que le c�ble est trop �tendu ou pas assez ?
    if( currentLen == m_length )
    {
        // Si non => pas besoin de collision
        return 0;
    }

    // C�ble trop �tendu ou pas assez => Cr�ation de collision
    contact->m_particles[ 0 ] = m_particles[ 0 ];
    contact->m_particles[ 1 ] = m_particles[ 1 ];

    // Calcul du vecteur normal
    Vector3 normal = m_particles[ 1 ]->getPosition() - m_particles[ 0 ]->getPosition();
    normal.normalize();

    // Le vecteur normal du contact d�pend du type de contrainte (compression ou extension)
    // Extension
    if( currentLen > m_length )
    {
        contact->m_contactNormal = normal;
        contact->m_penetration = currentLen - m_length;
    }
    // Compression
    else
    {
        contact->m_contactNormal = -normal;
        contact->m_penetration = m_length - currentLen;
    }

    // Pas de restitution (pas de "rebond") vu que c'est une tige bien solide et robuste
    contact->m_restitution = 0;

    return 1;
}
