#include "ParticleContactResolver.h"


ContactResolver::ContactResolver( unsigned iterations, float velocityEpsilon, float positionEpsilon )
{
    setIterations( iterations, iterations );
    setEpsilon( velocityEpsilon, positionEpsilon );
}

/**
 * @brief Résouds les collisions d'un ensemble de contacts entre particules (vélocité et pénétration)
 * @param contactArray 
 * @param numContacts 
 * @param duration 
*/
ContactResolver::ContactResolver( unsigned velocityIterations, unsigned positionIterations, float velocityEpsilon, float positionEpsilon )
{
    setIterations( velocityIterations );
    setEpsilon( velocityEpsilon, positionEpsilon );
}

void ContactResolver::setIterations( unsigned velocityIterations, unsigned positionIterations )
{
    ContactResolver::velocityIterations = velocityIterations;
    ContactResolver::positionIterations = positionIterations;
}

void ContactResolver::setIterations( unsigned iterations )
{
    setIterations( iterations, iterations );
}

void ContactResolver::setEpsilon( float velocityEpsilon, float positionEpsilon )
{
    ContactResolver::velocityEpsilon = velocityEpsilon;
    ContactResolver::positionEpsilon = positionEpsilon;
}

void ContactResolver::resolveContacts( std::vector<Contact>& contactArray, const int& numContacts, const float& duration )
{
    if( numContacts == 0 ) return;
    if( !isValid() ) return;

    // Prépare les collisions avant de les traiter
    prepareContacts( contactArray, numContacts, duration );

    // Résolution de l'interpénétration
    adjustPositions( contactArray, numContacts, duration );

    // Résolution de la vélocité
    adjustVelocities( contactArray, numContacts, duration );
}

void ContactResolver::prepareContacts( std::vector<Contact>& contacts, unsigned numContacts, float duration )
{
    // Génère la vélocité de la collision
    Contact* lastContact = &contacts[ numContacts ];
    for( Contact* contact = &contacts[0]; contact < lastContact; contact++ )
    {
        // Calcule les données internes à la collision (inertie, base)
        contact->calculateInternals( duration );
    }
}

void ContactResolver::adjustVelocities( std::vector<Contact>& c, unsigned numContacts, float duration )
{
    Vector3 velocityChange[ 2 ], rotationChange[ 2 ];
    Vector3 deltaVel;

    // Gestion des collisions selon leur criticité (vélocité de rapprochement supérieure = + critique)
    velocityIterationsUsed = 0;
    while( velocityIterationsUsed < velocityIterations )
    {
        // Trouve la collision avec la plus grande vélocité
        float max = velocityEpsilon;
        unsigned index = numContacts;
        for( unsigned i = 0; i < numContacts; i++ )
        {
            if( c[ i ].desiredDeltaVelocity > max )
            {
                max = c[ i ].desiredDeltaVelocity;
                index = i;
            }
        }
        if( index == numContacts ) break;

        // Résolution de vélocité sur la collision la plus critique
        c[ index ].applyVelocityChange( velocityChange, rotationChange );

        // Avec le changement de vélocité des deux corps, la vélocité de tous les corps des collisions peut nécessiter un recalcul (un corps pouvant se trouver dans plusieurs collisions)
        for( unsigned i = 0; i < numContacts; i++ )
        {
            // Pour chaque corps de la collision
            for( unsigned b = 0; b < 2; b++ ) if( c[ i ].m_rigidbodies[ b ] )
            {
                // Est-ce qu'il est égal à un des corps qui vient de subir la résolution
                for( unsigned d = 0; d < 2; d++ )
                {
                    if( c[ i ].m_rigidbodies[ b ] == c[ index ].m_rigidbodies[ d ] )
                    {
                        deltaVel = velocityChange[ d ] +
                            rotationChange[ d ] * c[ i ].relativeContactPosition[ b ];

                        // Le signe du changement est négatif si on gère le deuxième rigidbody de la collision
                        c[ i ].contactVelocity +=
                            c[ i ].contactToWorld.transformTranspose( deltaVel )
                            * ( b ? -1 : 1 );
                        c[ i ].calculateDesiredDeltaVelocity( duration );
                    }
                }
            }
        }
        velocityIterationsUsed++;
    }
}

void ContactResolver::adjustPositions( std::vector<Contact>& c, unsigned numContacts, float duration )
{
    unsigned i, index;
    Vector3 linearChange[ 2 ], angularChange[ 2 ];
    float max;
    Vector3 deltaPosition;

    // Résolution des interpénétrations selon la criticité
    positionIterationsUsed = 0;
    while( positionIterationsUsed < positionIterations )
    {
        // Trouve la plus grande pénétration
        max = positionEpsilon;
        index = numContacts;
        for( i = 0; i < numContacts; i++ )
        {
            if( c[ i ].m_penetration > max )
            {
                max = c[ i ].m_penetration;
                index = i;
            }
        }
        if( index == numContacts ) break;

        // Résolution d'interpénétration
        c[ index ].applyPositionChange(
            linearChange,
            angularChange,
            max );

        // La pénétration d'autres corps a pu être mise à jour via cette action, donc on doit la recalculer pour eux
        for( i = 0; i < numContacts; i++ )
        {
            // Pour chaque rb de la collision
            for( unsigned b = 0; b < 2; b++ ) if( c[ i ].m_rigidbodies[ b ] )
            {
                // On cherche si un des corps est le même qu'un des corps qui vient de subir la résolution
                for( unsigned d = 0; d < 2; d++ )
                {
                    if( c[ i ].m_rigidbodies[ b ] == c[ index ].m_rigidbodies[ d ] )
                    {
                        deltaPosition = linearChange[ d ] +
                            angularChange[ d ] * c[ i ].relativeContactPosition[ b ];

                        c[ i ].m_penetration +=
                            deltaPosition.dotProduct( c[ i ].m_contactNormal )
                            * ( b ? 1 : -1 );
                    }
                }
            }
        }
        positionIterationsUsed++;
    }
}
