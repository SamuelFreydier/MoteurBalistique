#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
protected:
    float m_inverseMass;
    float m_radius;

    bool m_showParticleInfos;
    Vector3 m_velocity;
    Vector3 m_acceleration;
    Vector3 m_position;
    Vector3 m_color;
    bool m_isStationary;

    // Facteur permettant de réduire la taille de la particule (< 1) ou de l'agrandir (> 1)
    float m_sizeModificator = 1.0;



    // Force résultante sur la particule
    struct accumForceSansGlitchFriction_t
    {
        Vector3 accumForce;
        bool glitchFriction = false;
    };

    accumForceSansGlitchFriction_t m_accumForce;

public:
    // Passe à true pour donner l'instruction à l'Engine de le détruire
    bool m_destroyedLater = false;
    bool isSelected = false;

    Particle( const float& mass = 1.0, const float& radius = 1.0, const Vector3& velocity = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& position = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& color = Vector3( { 255, 0, 0 } ), const bool& showParticleInfos = false);
    Particle( const Particle& particle );

    // Accesseurs et Mutateurs
    const float& getInverseMass() const { return m_inverseMass; }
    float getMass() const { return 1 / m_inverseMass; }
    void setMassReverse( const float& mass ) { m_inverseMass = 1 / mass; }

    const bool& getShowParticleInfos() const { return m_showParticleInfos; }
    void setShowParticleInfos( const bool& showParticleInfos ) { m_showParticleInfos = showParticleInfos; }

    const float& getRadius() const { return m_radius; }
    void setRadius(const float& radius) { m_radius = radius; }

    const Vector3& getVelocity() const { return m_velocity; }
    void setVelocity( const Vector3& velocity ) { m_velocity = velocity; }

    const Vector3& getAcceleration() const { return m_acceleration; }
    void setAcceleration( const Vector3& acceleration ) { m_acceleration = acceleration; }

    const Vector3& getPosition() const { return m_position; }
    void setPosition( const Vector3& position ) { m_position = position; }

    const Vector3& getColor() const { return m_color; }
    void setColor( const Vector3& color ) { m_color = color; }

    const float& getSizeModificator() const { return m_sizeModificator; }
    void setSizeModificator( const float& sizeModificator ) { m_sizeModificator = sizeModificator; }

    const bool& isStationary() const { return m_isStationary; }
    void setIsStationary(const bool isStationary) { m_isStationary = isStationary; }

    bool toBeDestroyed() const { return m_destroyedLater; }

    // Gestion des forces
    void addForce( const Vector3& forceVector, const bool& isFrictionGlitch = false);
    void clearAccum();

    // Mise à jour et affichage à chaque frame
    virtual void integrate( const float& secondsElapsedSincePreviousUpdate);
    void draw() const;
    virtual void clicked() {};


};

#endif
