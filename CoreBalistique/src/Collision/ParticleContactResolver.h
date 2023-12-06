#ifndef PARTICLECONTACTRESOLVER_H
#define PARTICLECONTACTRESOLVER_H

#include "ParticleContact.h"

/**
 * @brief Gère la résolution de toutes les collisions de particule
*/
class ContactResolver
{
protected:
    // Nombre d'itérations maximum
    int m_iterations;

    // Nombre d'itérations actuel
    int m_iterationsUsed;

public:
    ContactResolver( const int& iterations );

    void setIterations( const int& iterations ) { m_iterations = iterations; }
    void resolveContacts( std::vector<Contact>& contactArray, const int& numContacts, const float& duration );
};

#endif