#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle
{
protected:
    float m_massReverse;
    float m_radius;

    bool m_showParticleInfos;
    Vector3 m_velocity;
    Vector3 m_acceleration;
    Vector3 m_position;
    Vector3 m_color;

    // Facteur permettant de r�duire la taille de la particule (< 1) ou de l'agrandir (> 1)
    float m_sizeModificator = 1.0;

    // Passe � true pour donner l'instruction � l'Engine de le d�truire
    bool m_destroyedLater = false;


    const float masseVolumiqueAir = 1.2; // 1.2kg par m^3 pour air � 20�C
    const float coefSurfaceAeroDynamique = 0.70; // dans ces eaux-l� pour une sph�re mais � d�terminer en soufflerie

    // formule train�e : 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * VitesseRelativeMasseAir^2 en Newtons
    // donc
    const float constTrainee = 0.5 * masseVolumiqueAir * coefSurfaceAeroDynamique;
    // et il ne manquera plus qu'� multiplier par frontalSurface et la vitesse relative � la masse d'air au carr�

    // Force r�sultante sur la particule
    Vector3 m_accumForce;

public:
    Particle( const float& mass = 1.0, const float& radius = 1.0, const Vector3& velocity = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& position = Vector3( { 0.0, 0.0, 0.0 } ), const Vector3& color = Vector3( { 255, 0, 0 } ), const bool& showParticleInfos = false);
    Particle( const Particle& particle );

    // Accesseurs et Mutateurs
    const float& getMassReverse() const { return m_massReverse; }
    float getMass() const { return 1 / m_massReverse; }
    void setMassReverse( const float& mass ) { m_massReverse = 1 / mass; }

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

    bool toBeDestroyed() const { return m_destroyedLater; }

    // Gestion des forces
    void addForce( const Vector3& forceVector );
    void clearAccum();

    // Mise � jour et affichage � chaque frame
    virtual void update( const float& deltaTime );
    void draw() const;
    virtual void clicked() {};


};

#endif
