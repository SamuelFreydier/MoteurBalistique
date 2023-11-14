#ifndef PARTICLECONTACTGENERATOR_H
#define PARTICLECONTACTGENERATOR_H

#include "ParticleContact.h"

/**
 * @brief Interface pour divers types de g�n�rateurs de collisions
*/
class ParticleContactGenerator
{
public:
    virtual int addContact( ParticleContact* contact, const int& limit ) const = 0;
};

#endif