#ifndef PARTICLEROD_H
#define PARTICLEROD_H

#include "ParticleLink.h"

/**
 * @brief Cr�e une tige entre deux particules et g�n�re une collision si elles s'�loignent ou se rapprochent trop l'une de l'autre
*/
class ParticleRod : public ParticleLink
{
public:
    // Longueur de la tige
    float m_length;

public:
    // Cr�e une collision si n�cessaire pour �viter que la tige soit rompue
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif