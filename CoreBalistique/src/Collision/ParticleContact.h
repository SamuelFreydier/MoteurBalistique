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

    float m_friction;

    // Coefficient normal de restitution au niveau du contact
    float m_restitution;

    // Direction du contact (en suivant la perspective du premier objet)
    Vector3 m_contactNormal;

    // Profondeur de la pénétration au contact
    float m_penetration;

    // Point de contact
    Vector3 m_contactPoint;

    /**
         * A transform matrix that converts co-ordinates in the contact's
         * frame of reference to world co-ordinates. The columns of this
         * matrix form an orthonormal set of vectors.
         */
    Matrix3x3 contactToWorld;

    /**
     * Holds the closing velocity at the point of contact. This is set
     * when the calculateInternals function is run.
     */
    Vector3 contactVelocity;

    /**
     * Holds the required change in velocity for this contact to be
     * resolved.
     */
    float desiredDeltaVelocity;

    /**
     * Holds the world space position of the contact point relative to
     * centre of each body. This is set when the calculateInternals
     * function is run.
     */
    Vector3 relativeContactPosition[ 2 ];

    void setBodyData( std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two,
                      float friction, float restitution );

    /**
         * Calculates internal data from state data. This is called before
         * the resolution algorithm tries to do any resolution. It should
         * never need to be called manually.
         */
    void calculateInternals( float duration );

    /**
     * Reverses the contact. This involves swapping the two rigid bodies
     * and reversing the contact normal. The internal values should then
     * be recalculated using calculateInternals (this is not done
     * automatically).
     */
    void swapBodies();


    /**
     * Calculates and sets the internal value for the desired delta
     * velocity.
     */
    void calculateDesiredDeltaVelocity( float duration );

    /**
     * Calculates and returns the velocity of the contact
     * point on the given body.
     */
    Vector3 calculateLocalVelocity( unsigned bodyIndex, float duration );

    /**
     * Calculates an orthonormal basis for the contact point, based on
     * the primary friction direction (for anisotropic friction) or
     * a random orientation (for isotropic friction).
     */
    void calculateContactBasis();

    /**
     * Applies an impulse to the given body, returning the
     * change in velocities.
     */
    void applyImpulse( const Vector3& impulse, Rigidbody* body,
                       Vector3* velocityChange, Vector3* rotationChange );

    /**
     * Performs an inertia-weighted impulse based resolution of this
     * contact alone.
     */
    void applyVelocityChange( Vector3 velocityChange[ 2 ],
                              Vector3 rotationChange[ 2 ] );

    /**
     * Performs an inertia weighted penetration resolution of this
     * contact alone.
     */
    void applyPositionChange( Vector3 linearChange[ 2 ],
                              Vector3 angularChange[ 2 ],
                              float penetration );

    /**
     * Calculates the impulse needed to resolve this contact,
     * given that the contact has no friction. A pair of inertia
     * tensors - one for each contact object - is specified to
     * save calculation time: the calling function has access to
     * these anyway.
     */
    Vector3 calculateFrictionlessImpulse( Matrix3x3* inverseInertiaTensor );

    /**
     * Calculates the impulse needed to resolve this contact,
     * given that the contact has a non-zero coefficient of
     * friction. A pair of inertia tensors - one for each contact
     * object - is specified to save calculation time: the calling
     * function has access to these anyway.
     */
    Vector3 calculateFrictionImpulse( Matrix3x3* inverseInertiaTensor );

    // Résolution du contact pour la vélocité et l'interpénétration
    void resolve( const float& duration );

    // Calcul de la vélocité de séparation au contact
    float calculateClosingVelocity() const;

    // Gère les calculs d'impulsions pour la collision
    void resolveVelocity( const float& duration );

    // Gère la résolution d'interpénétration (overlap) pour la collision
    void resolveInterpenetration( const float& duration );

    // Calcule l'impulsion k à appliquer à chaque rigidbody
    float getContactImpulse(const float& duration) const;

    // Calcule la restitution à appliquer à cette collision en fonction de la situation
    float getRestitution() const;

    // Retourne la différence de vélocité entre les rb aux points de contact
    Vector3 getDiffVelocity() const;

    // Calcule la vélocité causée par l'accélération à la frame actuelle
    float getAccVelocity(const float& duration) const;

    // Retourne la partie du dénominateur de l'impulsion concernant un rb
    Vector3 getAngularImpulseDivider(std::shared_ptr<Rigidbody>) const;

    float getLinearInertia(std::shared_ptr<Rigidbody>) const;
    float getAngularInertia(std::shared_ptr<Rigidbody>) const;
};

#endif