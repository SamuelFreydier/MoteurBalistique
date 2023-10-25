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
    // liste des blobs
    std::vector<std::shared_ptr<Blob>>* m_blobs;

    // Longueur maximale du câble
    float m_maxLength;

    // Coefficient de restitution du câble
    float m_restitution;

public:
    //ParticleCable(Particle* particle1, Particle* particle2, float maxLength);
   

    // Pour initier le générateur de câble pour le comportement câble des ressorts des blobs
    void init(std::vector<std::shared_ptr<Blob>>* blobs);
    
    // Crée une collision si nécessaire pour éviter que le câble soit rompu
    virtual int addContact( ParticleContact* contact, const int& limit ) const override;
};

#endif