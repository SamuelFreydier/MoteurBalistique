#include "BlobContact.h"


// Pour initier le g�n�rateur de c�ble pour le comportement c�ble des ressorts des blobs
void BlobContact::init(std::vector<std::shared_ptr<Blob>>* blobs)
{
    m_blobs = blobs;
}


/**
* @brief G�n�re les collisions pour �viter que l'�lasticit� ne soit d�pass� pour chaque couple de blob
* @param contact
* @param limit (>= 1)
* @return 0 si le c�ble n'est pas sur-�tendu. 1 si une collision est n�cessaire.
*/
int BlobContact::addContact( ParticleContact* contact, const int& limit ) const
{
    // Les particules des blobs sont reli�es entre elles en utilisant des ressorts classiques.Par contre, ces liens ont une limite d��lasticit� au-del� 
    // de laquelle ils se comportent comme des c�bles
    int count = 0;
    for ( std::shared_ptr<Blob> blob : *m_blobs )
    {
        Blob::Partuples blobParticleAssociations = blob->getParticleAssociations();

        for (Blob::ParticleAssociation_t blobParticleAssociation : blobParticleAssociations)
        {
            std::shared_ptr<Particle> particle1 = blobParticleAssociation.firstParticle;
            std::shared_ptr<Particle> particle2 = blobParticleAssociation.secondParticle;

            ParticleCable CableContact;
            CableContact.m_maxLength = blobParticleAssociation.associationElasticity;
            CableContact.m_restitution = blobParticleAssociation.associationRestitution;
            CableContact.m_particles[ 0 ] = blobParticleAssociation.firstParticle;
            CableContact.m_particles[ 1 ] = blobParticleAssociation.secondParticle;
            
            int used = CableContact.addContact( contact, limit );
            count += used;
            contact += used;

            if (count >= limit)
            {
                break;
            }
        }

        if (count >= limit)
        {
            break;
        }
    }

    return count;
}