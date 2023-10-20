#include "GroundContacts.h"

void GroundContacts::init( Engine::Particles* particles )
{
    m_particles = particles;
}

int GroundContacts::addContact( ParticleContact* contact, const int& limit ) const
{
    int count = 0;
    for( Engine::Particles::iterator particleIterator = m_particles->begin(); particleIterator != m_particles->end(); particleIterator++ )
    {
        float y = ( *particleIterator )->getPosition().getY();
        float particleRadius = ( *particleIterator )->getRadius();
        float floorHeight = ofGetWindowHeight();
        if( y + particleRadius > floorHeight )
        {
            contact->m_contactNormal = Vector3( 0, 1, 0 );
            contact->m_particles[ 0 ] = *particleIterator;
            contact->m_particles[ 1 ] = nullptr;
            contact->m_penetration = y + particleRadius - floorHeight;
            contact->m_restitution = 0.f;
            contact++;
            count++;

        }

        if( count >= limit )
        {
            break;
        }
    }

    return count;
}
