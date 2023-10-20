#ifndef PARTICLECONTACTRESOLVER_H
#define PARTICLECONTACTRESOLVER_H

#include "ParticleContact.h"

/**
 * @brief G�re la r�solution de toutes les collisions de particule
*/
class ParticleContactResolver
{
protected:
    // Nombre d'it�rations maximum
    int m_iterations;

    // Nombre d'it�rations actuel
    int m_iterationsUsed;

public:
    ParticleContactResolver( const int& iterations );

    void setIterations( const int& iterations ) { m_iterations = iterations; }
    void resolveContacts( ParticleContact* contactArray, const int& numContacts, const float& duration );
};

#endif