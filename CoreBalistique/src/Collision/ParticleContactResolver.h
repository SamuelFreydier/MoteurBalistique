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

    // Nombre d'itérations pour la résolution de vélocité
    unsigned velocityIterations;

    // Nombre d'itérations pour la résolution de position
    unsigned positionIterations;

    // Les vélocités inférieures à cette valeur sont aplaties à 0
    float velocityEpsilon;

    // Les pénétrations inférieures à cette valeur sont ignorées
    float positionEpsilon;

public:
    // Itérations de vélocité utilisées dernièrement pour résoudre les collisions
    unsigned velocityIterationsUsed;

    // Itérations de position utilisées dernièrement pour résoudre les collisions
    unsigned positionIterationsUsed;

private:
    bool validSettings;

public:
    ContactResolver( unsigned iterations,
                     float velocityEpsilon = ( float ) 0.01,
                     float positionEpsilon = ( float ) 0.01 );

    ContactResolver( unsigned velocityIterations,
                     unsigned positionIterations,
                     float velocityEpsilon = ( float ) 0.01,
                     float positionEpsilon = ( float ) 0.01 );
    
    // Renvoie true si le resolver est prêt (paramètres valides)
    bool isValid()
    {
        return ( velocityIterations > 0 ) &&
            ( positionIterations > 0 ) &&
            ( positionEpsilon >= 0.0f ) &&
            ( positionEpsilon >= 0.0f );
    }

    // Met à jour le nombre d'itérations pour chaque étape de la résolution
    void setIterations( unsigned velocityIterations,
                        unsigned positionIterations );
   
    // Met à jour le nombre d'itérations qui est le même pour toutes les étapes
    void setIterations( unsigned iterations );

    // Met à jour la valeur charnière pour toutes les étapes
    void setEpsilon( float velocityEpsilon,
                     float positionEpsilon );

    // Résoud les collisions de contactArray
    void resolveContacts( std::vector<Contact>& contactArray, const int& numContacts, const float& duration );

protected:
    /**
        * Sets up contacts ready for processing. This makes sure their
        * internal data is configured correctly and the correct set of bodies
        * is made alive.
        */
    // Prépare les collisions et leurs données
    void prepareContacts( std::vector<Contact>& contactArray, unsigned numContacts,
                          float duration );

    /**
     * Resolves the velocity issues with the given array of constraints,
     * using the given number of iterations.
     */
    void adjustVelocities( std::vector<Contact>& contactArray,
                           unsigned numContacts,
                           float duration );

    /**
     * Resolves the positional issues with the given array of constraints,
     * using the given number of iterations.
     */
    void adjustPositions( std::vector<Contact>& contacts,
                          unsigned numContacts,
                          float duration );
};

#endif