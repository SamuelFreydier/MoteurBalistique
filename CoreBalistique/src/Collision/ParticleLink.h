#ifndef PARTICLELINK_H
#define PARTICLELINK_H

#include "ParticleContactGenerator.h"

/**
 * @brief Connecte deux particules, créant une collision si les contraintes du lien ne sont pas respectées.
 * Classe de base pour les câbles et les tiges
*/
class ParticleLink : public ParticleContactGenerator
{
public:
    // Paire de particules connectées
    std::shared_ptr<Particle> m_particles[ 2 ];

public:
    float currentLength() const;

    /**
     * @brief Génère les collisions pour éviter que le lien soit brisé.
     * @param contact 
     * @param limit (>= 1)
     * @return 0 si le lien est serein. 1 si une collision est nécessaire.
    */
    //ParticleLink(Particle* particle1, Particle* particle2); // Constructeur de classe abstraite n'a pas besoin d'etre virtuel. il n epourra néanmoins pas instancier d'objet ParticleLink car classe abstraite
    virtual int addContact( ParticleContact* contact, const int& limit ) const = 0;
};

#endif