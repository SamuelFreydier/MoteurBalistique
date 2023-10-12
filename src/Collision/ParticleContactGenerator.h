#ifndef PARTICLECONTACTGENERATOR_H
#define PARTICLECONTACTGENERATOR_H

#include "ParticleContact.h"

/**
 * @brief Interface pour divers types de générateurs de collisions
*/
class ParticleContactGenerator
{
public:
    virtual int addContact( std::shared_ptr<ParticleContact> contact, const int& limit ) const = 0;
};

#endif