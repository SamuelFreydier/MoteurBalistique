#include "ParticleContactResolver.h"


ContactResolver::ContactResolver( unsigned iterations, float velocityEpsilon, float positionEpsilon )
{
    setIterations( iterations, iterations );
    setEpsilon( velocityEpsilon, positionEpsilon );
}

/**
 * @brief R�souds les collisions d'un ensemble de contacts entre particules (v�locit� et p�n�tration)
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

    // Pr�pare les collisions avant de les traiter
    prepareContacts( contactArray, numContacts, duration );

    // R�solution de l'interp�n�tration
    adjustPositions( contactArray, numContacts, duration );

    // R�solution de la v�locit�
    adjustVelocities( contactArray, numContacts, duration );
}

void ContactResolver::prepareContacts( std::vector<Contact>& contacts, unsigned numContacts, float duration )
{
    // G�n�re la v�locit� de la collision
    Contact* lastContact = &contacts[ numContacts ];
    for( Contact* contact = &contacts[0]; contact < lastContact; contact++ )
    {
        // Calcule les donn�es internes � la collision (inertie, base)
        contact->calculateInternals( duration );
    }
}

void ContactResolver::adjustVelocities( std::vector<Contact>& c, unsigned numContacts, float duration )
{
    Vector3 velocityChange[ 2 ], rotationChange[ 2 ];
    Vector3 deltaVel;

    // Gestion des collisions selon leur criticit� (v�locit� de rapprochement sup�rieure = + critique)
    velocityIterationsUsed = 0;
    while( velocityIterationsUsed < velocityIterations )
    {
        // Trouve la collision avec la plus grande v�locit�
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

        // R�solution de v�locit� sur la collision la plus critique
        c[ index ].applyVelocityChange( velocityChange, rotationChange );

        // Avec le changement de v�locit� des deux corps, la v�locit� de tous les corps des collisions peut n�cessiter un recalcul (un corps pouvant se trouver dans plusieurs collisions)
        for( unsigned i = 0; i < numContacts; i++ )
        {
            // Pour chaque corps de la collision
            for( unsigned b = 0; b < 2; b++ ) if( c[ i ].m_rigidbodies[ b ] )
            {
                // Est-ce qu'il est �gal � un des corps qui vient de subir la r�solution
                for( unsigned d = 0; d < 2; d++ )
                {
                    if( c[ i ].m_rigidbodies[ b ] == c[ index ].m_rigidbodies[ d ] )
                    {
                        deltaVel = velocityChange[ d ] +
                            rotationChange[ d ] * c[ i ].relativeContactPosition[ b ];

                        // Le signe du changement est n�gatif si on g�re le deuxi�me rigidbody de la collision
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

    // R�solution des interp�n�trations selon la criticit�
    positionIterationsUsed = 0;
    while( positionIterationsUsed < positionIterations )
    {
        // Trouve la plus grande p�n�tration
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

        // R�solution d'interp�n�tration
        c[ index ].applyPositionChange(
            linearChange,
            angularChange,
            max );

        // La p�n�tration d'autres corps a pu �tre mise � jour via cette action, donc on doit la recalculer pour eux
        for( i = 0; i < numContacts; i++ )
        {
            // Pour chaque rb de la collision
            for( unsigned b = 0; b < 2; b++ ) if( c[ i ].m_rigidbodies[ b ] )
            {
                // On cherche si un des corps est le m�me qu'un des corps qui vient de subir la r�solution
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
