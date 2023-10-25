#include "ParticleCable.h"


/*
ParticleCable::ParticleCable(Particle* particle1, Particle* particle2, float maxLength)
    : ParticleLink(particle1, particle2), m_maxLength(maxLength)
{

}*/


// Pour initier le g�n�rateur de c�ble pour le comportement c�ble des ressorts des blobs
void ParticleCable::init(std::vector<std::shared_ptr<Blob>>* blobs)
{
    m_blobs = blobs;
}


/**
* @brief G�n�re les collisions pour �viter que le c�ble soit bris�.
* @param contact
* @param limit (>= 1)
* @return 0 si le c�ble n'est pas sur-�tendu. 1 si une collision est n�cessaire.
*/
int ParticleCable::addContact( ParticleContact* contact, const int& limit ) const
{
    // Les particules des blobs sont reli�es entre elles en utilisant des ressorts classiques.Par contre, ces liens ont une limite d��lasticit� au-del� 
    // de laquelle ils se comportent comme des c�bles.
    const float elasticityLimit = 15; // d�gueulasse
    const float coefRestitution = 1; // 0 = collision parfaitement in�lastique et 1 = collision parfaitement �lastique

    int count = 0;
    for ( std::shared_ptr<Blob> blob : *m_blobs)
    {
        Blob::Partuples blobParticleAssociations = blob->getParticleAssociations();

        for (Blob::ParticleAssociation_t blobParticleAssociation : blobParticleAssociations)
        {
            std::shared_ptr<Particle> particle1 = blobParticleAssociation.firstParticle;
            std::shared_ptr<Particle> particle2 = blobParticleAssociation.secondParticle;

            // Longueur du c�ble
            float length = particle1->getPosition().distance(particle2->getPosition());

            // Est-ce que le c�ble est trop �tendu ? Si non => pas besoin de collision
            if (length > elasticityLimit)
            {
                // C�ble trop �tendu => Cr�ation de collision
                contact->m_particles[0] = particle1;
                contact->m_particles[1] = particle2;

                // Calcul du vecteur normal
                Vector3 normal = -(particle2->getPosition() - particle1->getPosition());
                normal.normalize();
                contact->m_contactNormal = normal;
                contact->m_penetration = length - elasticityLimit;
                contact->m_restitution = coefRestitution;
                contact++;
                count++;
            }


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