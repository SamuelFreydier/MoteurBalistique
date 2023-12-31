#ifndef PARTICLECONTACTRESOLVER_H
#define PARTICLECONTACTRESOLVER_H

#include "ParticleContact.h"

/**
 * @brief G�re la r�solution de toutes les collisions de particule
*/
class ContactResolver
{
protected:
    // Nombre d'it�rations maximum
    int m_iterations;

    // Nombre d'it�rations actuel
    int m_iterationsUsed;

    // Nombre d'it�rations pour la r�solution de v�locit�
    unsigned velocityIterations;

    // Nombre d'it�rations pour la r�solution de position
    unsigned positionIterations;

    // Les v�locit�s inf�rieures � cette valeur sont aplaties � 0
    float velocityEpsilon;

    // Les p�n�trations inf�rieures � cette valeur sont ignor�es
    float positionEpsilon;

public:
    // It�rations de v�locit� utilis�es derni�rement pour r�soudre les collisions
    unsigned velocityIterationsUsed;

    // It�rations de position utilis�es derni�rement pour r�soudre les collisions
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
    
    // Renvoie true si le resolver est pr�t (param�tres valides)
    bool isValid();

    // Met � jour le nombre d'it�rations pour chaque �tape de la r�solution
    void setIterations( unsigned velocityIterations,
                        unsigned positionIterations );
   
    // Met � jour le nombre d'it�rations qui est le m�me pour toutes les �tapes
    void setIterations( unsigned iterations );

    // Met � jour la valeur charni�re pour toutes les �tapes
    void setEpsilon( float velocityEpsilon,
                     float positionEpsilon );

    // R�soud les collisions de contactArray
    void resolveContacts( std::vector<Contact>& contactArray, const int& numContacts, const float& duration );

protected:
    // Pr�pare les collisions et leurs donn�es
    void prepareContacts( std::vector<Contact>& contactArray, unsigned numContacts,
                          float duration );

    // R�solution de la v�locit� des collisions
    void adjustVelocities( std::vector<Contact>& contactArray,
                           unsigned numContacts,
                           float duration );

    // R�solution de l'interp�n�tration des collisions
    void adjustPositions( std::vector<Contact>& contacts,
                          unsigned numContacts,
                          float duration );
};

#endif