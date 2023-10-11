#ifndef FIREBALL_H
#define FIREBALL_H

#include "Particle.h"

class Fireball : public Particle
{
private:
    int m_colorShift = 20;
    int m_nbExplosionProjectiles = 5;
public:
    Fireball( const float& mass = 1.0, const float& radius = 1.0, const Vector& velocity = Vector( { 0.0, 0.0, 0.0 } ), const Vector& position = Vector( { 0.0, 0.0, 0.0 } ), const Vector& color = Vector( { 255, 0, 0 } ), const int& colorShift = 0 );
    Fireball( const Fireball& fireball );

    const float& getColorShift() const { return m_colorShift; }
    void setColorShift( const float& colorShift ) { m_colorShift = colorShift; }

    void update( const float& deltaTime ) override;
    void clicked() override;
    void explode();
};

#endif
