#ifndef PARTICLECONTACT_H
#define PARTICLECONTACT_H

#include "Particle.h"
#include "Rigidbody.h"

/**
 * @brief Repr�sente deux particules en contact
*/
class Contact
{
    friend class ContactResolver;

public:
    // Particules impliqu�es dans le contact. La seconde particule est � nullptr si la collision est entre l'objet et la sc�ne
    std::shared_ptr<Particle> m_particles[ 2 ];

    // Rigidbodies impliqu�s dans le contact. La seconde particule est � nullptr si la collision est entre l'objet et la sc�ne
    std::shared_ptr<Rigidbody> m_rigidbodies[2];

    // Coefficient de friction
    float m_friction;

    // Coefficient normal de restitution au niveau du contact
    float m_restitution;

    // Direction du contact (en suivant la perspective du premier objet)
    Vector3 m_contactNormal;

    // Profondeur de la p�n�tration au contact
    float m_penetration;

    // Point de contact
    Vector3 m_contactPoint;

    // Matrice de transformation qui convertit les coordonn�es dans le rep�re de la collision jusqu'au rep�re monde. La matrice est orthonormale
    Matrix3x3 contactToWorld;

    // Contient la v�locit� de rapprochement de la collision. D�fini dans calculateInternals
    Vector3 contactVelocity;

    // Changement de v�locit� requis pour que la collision soit r�solue
    float desiredDeltaVelocity;

    // Position monde du point de contact relatif au centre des deux corps. D�fini dans calculateInternals
    Vector3 relativeContactPosition[ 2 ];

    // D�finis les donn�es qui ne d�pendent pas de la position de la collision (le tableau des corps, la friction, la restitution)
    void setBodyData( std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two,
                      float friction, float restitution );

    // Calcule des donn�es internes pour pr�parer la collision. Est appel� avant la r�solution de la collision.
    void calculateInternals( float duration );

    // Echange les deux corps de la collision
    void swapBodies();

    // Calcule le changement de v�locit� voulu pour la r�solution
    void calculateDesiredDeltaVelocity( float duration );

    // Calcule et retourne la v�locit� au point de contact qui est appliqu�e sp�cifiquement sur un des deux corps
    Vector3 calculateLocalVelocity( unsigned bodyIndex, float duration );

    // Calcule d'une base orthonorm�e au point de contact (rep�re de la collision)
    void calculateContactBasis();


    // R�solution de la v�locit� (impulsion)
    void applyVelocityChange( Vector3 velocityChange[ 2 ],
                              Vector3 rotationChange[ 2 ] );

    // R�solution de l'interp�n�tration
    void applyPositionChange( Vector3 linearChange[ 2 ],
                              Vector3 angularChange[ 2 ],
                              float penetration );


    // Calcule l'impulsion n�cessaire pour la r�solution de la collision, sans prendre en compte la friction
    Vector3 calculateFrictionlessImpulse( Matrix3x3* inverseInertiaTensor );

    // Calcule l'impulsion n�cessaire pour la r�solution de la collision, en prenant en compte la friction
    Vector3 calculateFrictionImpulse( Matrix3x3* inverseInertiaTensor );

    // Calcule la restitution � appliquer � cette collision en fonction de la situation
    float getRestitution() const;
};

#endif