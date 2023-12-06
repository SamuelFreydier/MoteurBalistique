#ifndef PARTICLEROD_H
#define PARTICLEROD_H

#include "ParticleLink.h"

/**
 * @brief Crée une tige entre deux particules et génère une collision si elles s'éloignent ou se rapprochent trop l'une de l'autre
*/
class ParticleRod : public ParticleLink
{
public:
    // Longueur de la tige
    float m_length;

public:
    // Crée une collision si nécessaire pour éviter que la tige soit rompue
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif