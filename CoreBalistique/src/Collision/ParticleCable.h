#ifndef PARTICLECABLE_H
#define PARTICLECABLE_H

#include "ParticleLink.h"
#include "Blob.h"

/**
 * @brief Crée un câble entre deux particules et génère une collision si elles s'éloignent trop l'une de l'autre
*/
class ParticleCable : public ParticleLink
{
public:
    // Longueur maximale du câble
    float m_maxLength;

    // Coefficient de restitution du câble
    float m_restitution;

public:
    // Crée une collision si nécessaire pour éviter que le câble soit rompu
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif