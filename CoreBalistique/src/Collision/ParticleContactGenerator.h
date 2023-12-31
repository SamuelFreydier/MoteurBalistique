#ifndef PARTICLECONTACTGENERATOR_H
#define PARTICLECONTACTGENERATOR_H

#include "ParticleContact.h"

/**
 * @brief Interface pour divers types de générateurs de collisions
*/
class ParticleContactGenerator
{
public:
    virtual int addContact( Contact* contact, const int& limit ) const = 0;
};

#endif