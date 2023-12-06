#ifndef PARTICLECABLE_H
#define PARTICLECABLE_H

#include "ParticleLink.h"
#include "Blob.h"

/**
 * @brief Cr�e un c�ble entre deux particules et g�n�re une collision si elles s'�loignent trop l'une de l'autre
*/
class ParticleCable : public ParticleLink
{
public:
    // Longueur maximale du c�ble
    float m_maxLength;

    // Coefficient de restitution du c�ble
    float m_restitution;

public:
    // Cr�e une collision si n�cessaire pour �viter que le c�ble soit rompu
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif