#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"
#include "Rigidbody.h"

/**
 * @brief Repr�sente deux particules en contact
*/
class ParticleContact
{
    friend class ParticleContactResolver;

public:
    // Particules impliqu�es dans le contact. La seconde particule est � nullptr si la collision est entre l'objet et la sc�ne
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Rigidbodies impliqu�s dans le contact. La seconde particule est � nullptr si la collision est entre l'objet et la sc�ne
    std::shared_ptr<Rigidbody> m_rigidbodies[2];

    // Coefficient normal de restitution au niveau du contact
    float m_restitution;

    // Direction du contact (en suivant la perspective du premier objet)
    Vector3 m_contactNormal;

    // Profondeur de la p�n�tration au contact
    float m_penetration;

    // Point de contact
    Vector3 m_contactPoint;

    // R�solution du contact pour la v�locit� et l'interp�n�tration
    void resolve( const float& duration );

    // Calcul de la v�locit� de s�paration au contact
    float calculateClosingVelocity() const;

    // G�re les calculs d'impulsions pour la collision
    void resolveVelocity( const float& duration );

    // G�re la r�solution d'interp�n�tration (overlap) pour la collision
    void resolveInterpenetration( const float& duration );

};

#endif