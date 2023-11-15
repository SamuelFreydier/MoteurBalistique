#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector.h"
#include "Quaternion.h"

class Rigidbody
{
protected:
    // Inverse de la masse du corps rigide
    float m_inverseMass;

    // Montant de freinage appliqué au mouvement
    float m_linearDamping;
    
    // Position du corps rigide
    Vector3 m_position;

    // Orientation du corps rigide
    Quaternion m_orientation;

    // Vélocité linéaire du corps
    Vector3 m_velocity;

    // Vélocité angulaire du corps
    Vector3 m_angularVelocity;

    // Accélération linéaire du corps
    Vector3 m_acceleration;

    // Couleur du corps
    Vector3 m_color;

    // Force résultante sur la particule
    Vector3 m_accumForce;

public:
    // Passe à true pour donner l'instruction à l'Engine de le détruire
    bool m_destroyedLater = false;
    bool isSelected = false;

    Rigidbody( const float& mass = 1.0, const Vector3& velocity = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& position = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& color = Vector3( { 255, 0, 0 } ) );
    Rigidbody( const Rigidbody& particle );

    // Accesseurs et Mutateurs
    const float& getInverseMass() const { return m_inverseMass; }
    float getMass() const { return 1 / m_inverseMass; }
    void setMassReverse(const float& mass);

    const Vector3& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector3& velocity ) { m_velocity = velocity; }

    const Vector3& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector3& acceleration ) { m_acceleration = acceleration; }

    const Vector3& getPosition() const { return m_position; }
    void setPosition( const Vector3& position ) { m_position = position; }

    const Vector3& getColor() const { return m_color; }
    void setColor( const Vector3& color ) { m_color = color; }

    const Vector3& getAccumForce() const { return m_accumForce; }

    bool toBeDestroyed() const { return m_destroyedLater; }

    // Gestion des forces
    void addForce( const Vector3& forceVector );
    void clearAccum();

    // Mise à jour et affichage à chaque frame
    virtual void integrate( const float& secondsElapsedSincePreviousUpdate);
    void draw() const;

};

#endif
