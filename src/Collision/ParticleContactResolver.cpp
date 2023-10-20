#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver( const int& iterations )
    : m_iterations( iterations ), m_iterationsUsed( 0 )
{
}

/**
 * @brief R�souds les collisions d'un ensemble de contacts entre particules (v�locit� et p�n�tration)
 * @param contactArray 
 * @param numContacts 
 * @param duration 
*/
void ParticleContactResolver::resolveContacts( ParticleContact* contactArray, const int& numContacts, const float& duration )
{
    int index;

    m_iterationsUsed = 0;

    while( m_iterationsUsed < m_iterations )
    {
        // Trouve le contact avec la plus grande v�locit� de rapprochement (=> contact le plus critique)
        float max = 0;
        int maxIndex = numContacts;

        for( index = 0; index < numContacts; index++ )
        {
            float sepVelocity = contactArray[ index ].calculateSeparatingVelocity();
            if( sepVelocity > max &&
                ( sepVelocity > 0 || contactArray[ index ].m_penetration > 0 ) )
            {
                max = sepVelocity;
                maxIndex = index;
            }
        }

        // Aucune collision � r�soudre => on arr�te
        if( maxIndex == numContacts )
        {
            break;
        }

        // R�solution de la collision
        contactArray[ maxIndex ].resolve( duration );

        m_iterationsUsed++;
    }
}
