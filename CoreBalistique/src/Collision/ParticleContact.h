#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"
#include "Rigidbody.h"

/**
 * @brief Représente deux particules en contact
*/
class ParticleContact
{
    friend class ParticleContactResolver;

public:
    // Particules impliquées dans le contact. La seconde particule est à nullptr si la collision est entre l'objet et la scène
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Rigidbodies impliqués dans le contact. La seconde particule est à nullptr si la collision est entre l'objet et la scène
    std::shared_ptr<Rigidbody> m_rigidbodies[2];

    // Coefficient normal de restitution au niveau du contact
    float m_restitution;

    // Direction du contact (en suivant la perspective du premier objet)
    Vector3 m_contactNormal;

    // Profondeur de la pénétration au contact
    float m_penetration;

    // Point de contact
    Vector3 m_contactPoint;

    // Résolution du contact pour la vélocité et l'interpénétration
    void resolve( const float& duration );

    // Calcul de la vélocité de séparation au contact
    float calculateClosingVelocity() const;

    // Gère les calculs d'impulsions pour la collision
    void resolveVelocity( const float& duration );

    // Gère la résolution d'interpénétration (overlap) pour la collision
    void resolveInterpenetration( const float& duration );

};

#endif