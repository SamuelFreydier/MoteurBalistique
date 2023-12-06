#ifndef BLOBCONTACT_H
#define BLOBCONTACT_H

#include "ParticleCable.h"
#include "Blob.h"

/**
 * @brief Crée des câbles entre les blobs trop éloignés au delà d'une certaine élasticité
*/
class BlobContact : public ParticleContactGenerator
{
public:
    // liste des blobs
    std::vector<std::shared_ptr<Blob>>* m_blobs;

public:
   
    // Pour initier le générateur de câble pour le comportement câble des ressorts des blobs
    void init(std::vector<std::shared_ptr<Blob>>* blobs);
    
    // Crée une collision si nécessaire pour éviter que le câble soit rompu
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif