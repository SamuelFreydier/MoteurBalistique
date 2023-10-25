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
    // liste des blobs
    std::vector<std::shared_ptr<Blob>>* m_blobs;

    // Longueur maximale du c�ble
    float m_maxLength;

    // Coefficient de restitution du c�ble
    float m_restitution;

public:
    //ParticleCable(Particle* particle1, Particle* particle2, float maxLength);
   

    // Pour initier le g�n�rateur de c�ble pour le comportement c�ble des ressorts des blobs
    void init(std::vector<std::shared_ptr<Blob>>* blobs);
    
    // Cr�e une collision si n�cessaire pour �viter que le c�ble soit rompu
    virtual int addContact( ParticleContact* contact, const int& limit ) const override;
};

#endif