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

protected:
    float currentLength() const;

public:
    /**
     * @brief G�n�re les collisions pour �viter que le lien soit bris�.
     * @param contact 
     * @param limit (>= 1)
     * @return 0 si le lien est serein. 1 si une collision est n�cessaire.
    */
    virtual int addContact( std::shared_ptr<ParticleContact> contact, const int& limit ) const = 0;
};

#endif