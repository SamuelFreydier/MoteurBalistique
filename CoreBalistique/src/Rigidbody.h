#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

class Rigidbody
{
protected:
    // Inverse de la masse du corps rigide
    float m_inverseMass;

    // Montant de freinage appliqué au mouvement linéaire
    float m_linearDamping = 1;

    // Montant de freinage appliqué au mouvement rotationnel
    float m_angularDamping = 1;
    
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
    Vector3 m_lastFrameAcceleration;

    // Couleur du corps
    Vector3 m_color;

    // Force résultante sur le corps
    Vector3 m_accumForce;

    // Torque résultant sur le corps
    Vector3 m_accumTorque;

    // Matrice de conversion des coordonnées objet en coordonnées monde (et inversement)
    Matrix4x4 m_transformMatrix;

    // Tenseur d'inertie inverse (J^-1)
    Matrix3x3 m_inverseInertiaTensor;

    // Tenseur d'inertie inverse en coordonnées monde
    Matrix3x3 m_inverseInertiaTensorWorld;

public:
    // Passe à true pour donner l'instruction à l'Engine de le détruire
    bool m_destroyedLater = false;
    bool isSelected = false;

    Rigidbody( const float& mass = 1.0, const Vector3& velocity = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& position = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& angularVelocity = Vector3({0.0, 0.0, 0.0}), const Vector3& color = Vector3({255, 0, 0}));
    Rigidbody( const Rigidbody& rigidbody );

    // Accesseurs et Mutateurs
    const float& getInverseMass() const { return m_inverseMass; }
    float getMass() const { return 1 / m_inverseMass; }
    void setMassReverse(const float& mass);

    const float& getLinearDamping() const { return m_linearDamping; }
    void setLinearDamping( const float& damping ) { m_linearDamping = damping; }

    const float& getAngularDamping() const { return m_angularDamping; }
    void setAngularDamping( const float& damping ) { m_angularDamping = damping; }

    const Vector3& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector3& velocity ) { m_velocity = velocity; }

    const Vector3& getAngularVelocity() const { return m_angularVelocity; }
    void setAngularVelocity( const Vector3& angularVelocity ) { m_angularVelocity = angularVelocity; }

    const Vector3& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector3& acceleration ) { m_acceleration = acceleration; }

    const Vector3& getLastFrameAcceleration() const { return m_lastFrameAcceleration; }
    void setLastFrameAcceleration( const Vector3& lastFrameAcceleration ) { m_lastFrameAcceleration = lastFrameAcceleration; }

    const Vector3& getPosition() const { return m_position; }
    void setPosition( const Vector3& position ) { m_position = position; }

    const Quaternion& getOrientation() const { return m_orientation; }
    void setOrientation( const Quaternion& orientation ) { m_orientation = orientation; }

    const Vector3& getColor() const { return m_color; }
    void setColor( const Vector3& color ) { m_color = color; }

    const Vector3& getAccumForce() const { return m_accumForce; }
    const Vector3& getAccumTorque() const { return m_accumTorque; }

    const Matrix4x4& getTransformMatrix() const { return m_transformMatrix; }
    void setTransformMatrix( const Matrix4x4& matrix ) { m_transformMatrix = matrix; }

    const Matrix3x3& getInverseInertiaTensor() const { return m_inverseInertiaTensor; }
    const Matrix3x3& getInverseInertiaTensorWorld() const { return m_inverseInertiaTensorWorld; }
    void setInertiaTensor( const Matrix3x3& inertiaTensor );

    bool toBeDestroyed() const { return m_destroyedLater; }

    // Gestion des coordonnées des points
    Vector3 getPointInLocalSpace( const Vector3& point ) const;
    Vector3 getPointInWorldSpace( const Vector3& point ) const;

    // Gestion des forces
    void addForce( const Vector3& forceVector );
    void addForceAtBodyPoint( const Vector3& force, const Vector3& point );
    void addForceAtPoint( const Vector3& force, const Vector3& point );
    void clearAccum();

    // Gestion des données dérivées (pratiques mais techniquement redondantes)
    void calculateDerivedData();
    void calculateTransformMatrix( Matrix4x4& transformMatrix, const Vector3& position, const Quaternion& orientation );
    void transformInertiaTensor( Matrix3x3& iitWorld, const Quaternion& q, const Matrix3x3& iitBody, const Matrix4x4& rotmat );

    // Mise à jour physique du rigidbody
    void integrate( const float& secondsElapsedSincePreviousUpdate );

    // Affichage du rigidbody
    virtual void draw() const {}
};

#endif
