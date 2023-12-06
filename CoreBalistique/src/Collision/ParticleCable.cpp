#include "ParticleCable.h"

/**
* @brief G�n�re les collisions pour �viter que le c�ble soit bris�.
* @param contact
* @param limit (>= 1)
* @return 0 si le c�ble n'est pas sur-�tendu. 1 si une collision est n�cessaire.
*/
/*
int ParticleCable::addContact( Contact* contact, const int& limit ) const
{
    // Longueur du c�ble
    float length = currentLength();

    // Est-ce que le c�ble est trop �tendu ?
    if( length <= m_maxLength )
    {
        // Si non => pas besoin de collision
        return 0;
    }

    // C�ble trop �tendu => Cr�ation de collision
    contact->m_particles[ 0 ] = m_particles[ 0 ];
    contact->m_particles[ 1 ] = m_particles[ 1 ];

    // Calcul du vecteur normal
    Vector3 normal = -(m_particles[ 1 ]->getPosition() - m_particles[ 0 ]->getPosition());
    normal.normalize();
    contact->m_contactNormal = normal;
    contact->m_penetration = length - m_maxLength;
    contact->m_restitution = m_restitution;

    return 1;
}
*/


/**
* @brief G�n�re les collisions pour �viter que le c�ble soit bris�.
* @param contact
* @param limit (>= 1)
* @return 0 si le c�ble n'est pas sur-�tendu. 1 si une collision est n�cessaire.
*/
int ParticleCable::addContact(Contact* contact, const int& limit) const
{
    // Longueur du c�ble
    float length = currentLength();

    // Est-ce que le c�ble est trop �tendu ?
    if (length <= m_maxLength)
    {
        // Si non => pas besoin de collision
        return 0;
    }

    // C�ble trop �tendu => Cr�ation de collision
    contact->m_rigidbodies[0] = m_rigidbodies[0];
    contact->m_rigidbodies[1] = m_rigidbodies[1];

    // Calcul du vecteur normal
    Vector3 normal = -(m_rigidbodies[1]->getPosition() - m_rigidbodies[0]->getPosition());
    normal.normalize();
    contact->m_contactNormal = normal;
    contact->m_penetration = length - m_maxLength;
    contact->m_restitution = m_restitution;

    return 1;
}