#ifndef BLOBCONTACT_H
#define BLOBCONTACT_H

#include "ParticleCable.h"
#include "Blob.h"

/**
 * @brief Cr�e des c�bles entre les blobs trop �loign�s au del� d'une certaine �lasticit�
*/
class BlobContact : public ParticleContactGenerator
{
public:
    // liste des blobs
    std::vector<std::shared_ptr<Blob>>* m_blobs;

public:
   
    // Pour initier le g�n�rateur de c�ble pour le comportement c�ble des ressorts des blobs
    void init(std::vector<std::shared_ptr<Blob>>* blobs);
    
    // Cr�e une collision si n�cessaire pour �viter que le c�ble soit rompu
    virtual int addContact( Contact* contact, const int& limit ) const override;
};

#endif