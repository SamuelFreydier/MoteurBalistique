#ifndef GR0UNDCONTACTS_H
#define GR0UNDCONTACTS_H

#include "ParticleContactGenerator.h"
#include "Engine.h"

/**
 * @brief G�n�re la collision entre des particules et un sol
*/
class GroundContacts : public ParticleContactGenerator
{
    private:
        // Ensemble de particules concern�es par la collision au sol
        Engine::Particles* m_particles;

    public:
        void init( Engine::Particles* particles );

        virtual int addContact( ParticleContact* contact, const int& limit ) const override;
};

#endif