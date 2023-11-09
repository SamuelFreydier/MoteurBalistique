#ifndef PARTICLELINK_H
#define PARTICLELINK_H

#include "ParticleContactGenerator.h"

/**
 * @brief Connecte deux particules, cr�ant une collision si les contraintes du lien ne sont pas respect�es.
 * Classe de base pour les c�bles et les tiges
*/
class ParticleLink : public ParticleContactGenerator
{
public:
    // Paire de particules connect�es
    std::shared_ptr<Particle> m_particles[ 2 ];

public:
    float currentLength() const;

    /**
     * @brief G�n�re les collisions pour �viter que le lien soit bris�.
     * @param contact 
     * @param limit (>= 1)
     * @return 0 si le lien est serein. 1 si une collision est n�cessaire.
    */
    //ParticleLink(Particle* particle1, Particle* particle2); // Constructeur de classe abstraite n'a pas besoin d'etre virtuel. il n epourra n�anmoins pas instancier d'objet ParticleLink car classe abstraite
    virtual int addContact( ParticleContact* contact, const int& limit ) const = 0;
};

#endif