#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
protected:
    float m_massReverse;
    float m_radius;

    Vector m_velocity;
    Vector m_acceleration;
    Vector m_position;
    Vector m_color;

    // Facteur permettant de réduire la taille de la particule (< 1) ou de l'agrandir (> 1)
    float m_sizeModificator = 1.0;

    // Passe à true pour donner l'instruction à l'Engine de le détruire
    bool m_destroyedLater = false;

    // Force résultante sur la particule
    Vector m_accumForce;

public:
    Particle( const float& mass = 1.0, const float& radius = 1.0, const Vector& velocity = Vector( { 0.0, 0.0, 0.0 } ), const Vector& acceleration = Vector( { 0.0, 0.0, 0.0 } ), const Vector& position = Vector( { 0.0, 0.0, 0.0 } ), const Vector& color = Vector( { 255, 0, 0 } ) );
    Particle( const Particle& particle );

    // Accesseurs et Mutateurs
    const float& getMassReverse() const { return m_massReverse; }
    float getMass() const { return 1 / m_massReverse; }
    void setMassReverse( const float& mass ) { m_massReverse = 1 / mass; }

    const float& getRadius() const { return m_radius; }
    void setRadius( const float& radius ) { m_radius = radius; }

    const Vector& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector& velocity ) { m_acceleration = velocity; }

    const Vector& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector& acceleration ) { m_acceleration = acceleration; }

    const Vector& getPosition() const { return m_position; }
    void setPosition( const Vector& position ) { m_position = position; }

    const Vector& getColor() const { return m_color; }
    void setColor( const Vector& color ) { m_color = color; }

    const float& getSizeModificator() const { return m_sizeModificator; }
    void setSizeModificator( const float& sizeModificator ) { m_sizeModificator = sizeModificator; }

    bool toBeDestroyed() const { return m_destroyedLater; }

    // Gestion des forces
    void addForce( const Vector& forceVector );
    void clearAccum();

    // Mise à jour et affichage à chaque frame
    virtual void update( const float& deltaTime );
    void draw() const;
    virtual void clicked() {};
};

#endif
