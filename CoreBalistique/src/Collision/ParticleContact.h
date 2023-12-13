#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"
#include "Rigidbody.h"

/**
 * @brief Représente deux particules en contact
*/
class Contact
{
    friend class ContactResolver;

public:
    // Particules impliquées dans le contact. La seconde particule est à nullptr si la collision est entre l'objet et la scène
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Rigidbodies impliqués dans le contact. La seconde particule est à nullptr si la collision est entre l'objet et la scène
    std::shared_ptr<Rigidbody> m_rigidbodies[2];

    // Coefficient de friction
    float m_friction;

    // Coefficient normal de restitution au niveau du contact
    float m_restitution;

    // Direction du contact (en suivant la perspective du premier objet)
    Vector3 m_contactNormal;

    // Profondeur de la pénétration au contact
    float m_penetration;

    // Point de contact
    Vector3 m_contactPoint;

    // Matrice de transformation qui convertit les coordonnées dans le repère de la collision jusqu'au repère monde. La matrice est orthonormale
    Matrix3x3 contactToWorld;

    // Contient la vélocité de rapprochement de la collision. Défini dans calculateInternals
    Vector3 contactVelocity;

    // Changement de vélocité requis pour que la collision soit résolue
    float desiredDeltaVelocity;

    // Position monde du point de contact relatif au centre des deux corps. Défini dans calculateInternals
    Vector3 relativeContactPosition[ 2 ];

    // Définis les données qui ne dépendent pas de la position de la collision (le tableau des corps, la friction, la restitution)
    void setBodyData( std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two,
                      float friction, float restitution );

    // Calcule des données internes pour préparer la collision. Est appelé avant la résolution de la collision.
    void calculateInternals( float duration );

    // Echange les deux corps de la collision
    void swapBodies();

    // Calcule le changement de vélocité voulu pour la résolution
    void calculateDesiredDeltaVelocity( float duration );

    // Calcule et retourne la vélocité au point de contact qui est appliquée spécifiquement sur un des deux corps
    Vector3 calculateLocalVelocity( unsigned bodyIndex, float duration );

    // Calcule d'une base orthonormée au point de contact (repère de la collision)
    void calculateContactBasis();


    // Résolution de la vélocité (impulsion)
    void applyVelocityChange( Vector3 velocityChange[ 2 ],
                              Vector3 rotationChange[ 2 ] );

    // Résolution de l'interpénétration
    void applyPositionChange( Vector3 linearChange[ 2 ],
                              Vector3 angularChange[ 2 ],
                              float penetration );


    // Calcule l'impulsion nécessaire pour la résolution de la collision, sans prendre en compte la friction
    Vector3 calculateFrictionlessImpulse( Matrix3x3* inverseInertiaTensor );

    // Calcule l'impulsion nécessaire pour la résolution de la collision, en prenant en compte la friction
    Vector3 calculateFrictionImpulse( Matrix3x3* inverseInertiaTensor );

    // Calcule la restitution à appliquer à cette collision en fonction de la situation
    float getRestitution() const;
};

#endif