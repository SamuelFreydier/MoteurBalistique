#include "ParticleCable.h"


/*
ParticleCable::ParticleCable(Particle* particle1, Particle* particle2, float maxLength)
    : ParticleLink(particle1, particle2), m_maxLength(maxLength)
{

}*/


// Pour initier le générateur de câble pour le comportement câble des ressorts des blobs
void ParticleCable::init(std::vector<std::shared_ptr<Blob>>* blobs)
{
    m_blobs = blobs;
}


/**
* @brief Génère les collisions pour éviter que le câble soit brisé.
* @param contact
* @param limit (>= 1)
* @return 0 si le câble n'est pas sur-étendu. 1 si une collision est nécessaire.
*/
int ParticleCable::addContact( ParticleContact* contact, const int& limit ) const
{
    // Les particules des blobs sont reliées entre elles en utilisant des ressorts classiques.Par contre, ces liens ont une limite d’élasticité au-delà 
    // de laquelle ils se comportent comme des câbles.
    const float elasticityLimit = 15; // dégueulasse
    const float coefRestitution = 1; // 0 = collision parfaitement inélastique et 1 = collision parfaitement élastique

    int count = 0;
    for ( std::shared_ptr<Blob> blob : *m_blobs)
    {
        Blob::Partuples blobParticleAssociations = blob->getParticleAssociations();

        for (Blob::ParticleAssociation_t blobParticleAssociation : blobParticleAssociations)
        {
            std::shared_ptr<Particle> particle1 = blobParticleAssociation.firstParticle;
            std::shared_ptr<Particle> particle2 = blobParticleAssociation.secondParticle;

            // Longueur du câble
            float length = particle1->getPosition().distance(particle2->getPosition());

            // Est-ce que le câble est trop étendu ? Si non => pas besoin de collision
            if (length > elasticityLimit)
            {
                // Câble trop étendu => Création de collision
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