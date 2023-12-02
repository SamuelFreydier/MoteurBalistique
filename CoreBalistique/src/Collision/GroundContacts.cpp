#include "GroundContacts.h"

void GroundContacts::init( Engine::Particles* particles, Engine::Rigidbodies* rigidbodies )
{
    m_particles = particles;
    m_rigidbodies = rigidbodies;
}

int GroundContacts::addContact( ParticleContact* contact, const int& limit ) const
{
    int count = 0;
    float floorHeight = 0;
    Vector3 planeDirection = Vector3(0, -1, 0);

    // Particules
    for( Engine::Particles::iterator particleIterator = m_particles->begin(); particleIterator != m_particles->end(); particleIterator++ )
    {
        float y = ( *particleIterator )->getPosition().getY();
        float particleRadius = ( *particleIterator )->getRadius();
        if( y - particleRadius < floorHeight )
        {
            contact->m_contactNormal = planeDirection;
            contact->m_particles[ 0 ] = *particleIterator;
            contact->m_particles[ 1 ] = nullptr;
            contact->m_penetration = floorHeight - (y - particleRadius);
            contact->m_restitution = 0.7f;
            contact++;
            count++;
        }

        if( count >= limit )
        {
            break;
        }
    }

    // Rigidbodies
    float vertexDistance;
    for (Engine::Rigidbodies::iterator rbIterator = m_rigidbodies->begin(); rbIterator != m_rigidbodies->end(); rbIterator++)
    {
        // On compte le nombre de sommets à prendre en compte dans la collision
        for (Vector3 vertexPos : (*rbIterator)->getVerticesGlobalPosition()) 
        {
            vertexDistance = vertexPos.y;
            if (vertexDistance < floorHeight)
            {
                // Données de contact
                contact->m_rigidbodies[0] = *rbIterator;
                contact->m_rigidbodies[1] = nullptr;
                contact->m_contactPoint = (planeDirection * (vertexDistance - floorHeight)) + vertexPos;
                contact->m_contactNormal = planeDirection;
                contact->m_penetration = vertexDistance - floorHeight;
                contact->m_restitution = 0.7f;
                contact++;
                count++;
            }
        }

        if (count >= limit)
        {
            break;
        }
    }

    return count;
}
