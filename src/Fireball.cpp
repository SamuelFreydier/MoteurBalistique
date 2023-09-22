#include "Fireball.h"
#include "Engine.h"

Fireball::Fireball( const float& mass, const float& radius, bool hasTrail, const Vector& velocity, const Vector& acceleration, const Vector& position, const Vector& color, const int& colorShift )
    : Particle( mass, radius, hasTrail, velocity, acceleration, position, color ), m_colorShift( colorShift )
{

}

Fireball::Fireball( const Fireball& fireball )
    : Particle( fireball ), m_colorShift( fireball.getColorShift() )
{

}

void Fireball::update( const float& deltaTime )
{
    Vector lastPosition( getPosition() );

    Particle::update( deltaTime );

    int nbAshfalls = rand() % 2 + 3; // Entre 3 et 4 retombées de cendres

    for( int ashfallIdx = 0; ashfallIdx < nbAshfalls; ashfallIdx++ )
    {
        Vector colorShift( Engine::randshiftColor( m_color, m_colorShift ) );

        float traveledDistance = lastPosition.distance( getPosition() );
        Vector positionShift( getPosition() - lastPosition );
        positionShift *= 1 - m_radius / traveledDistance;

        Vector newPosition( lastPosition + positionShift );

        float angle = atan2( getPosition().getY() - newPosition.getY(), getPosition().getX() - newPosition.getX() ) + PI;
        angle +=  ( rand() % ( int ) ( PI * 100 + 1 ) ) / 100.0 - PI / 2;
        newPosition[ 0 ] += cos( angle ) * m_radius;
        newPosition[ 1 ] += sin( angle ) * m_radius;

        ParticlePtr ashfall = std::make_shared<Particle>( getMass() * 0.1, getRadius() * 0.3, false, Vector( { 0, 0, 0 } ), getAcceleration(), newPosition, colorShift );

        // Disparition progressive de la trainée de la boule de feu
        ashfall->setSizeModificator( ( rand() % 8 + 85 ) / 100.0 );
        Engine::getInstance()->addVanillaParticle( ashfall );
    }
}

void Fireball::clicked()
{
    explode();
}

void Fireball::explode()
{
    float angleStep = PI * 2 / m_nbExplosionProjectiles;

    // Une itération = un débris qui part dans une direction
    for( float angleProjection = 0.0; angleProjection < 2 * PI; angleProjection += angleStep )
    {
        Engine::getInstance()->shootParticle( getPosition(), angleProjection, getVelocity().norm(), getMass() * 0.4, getRadius() * 0.4, true, m_color, true );
    }

    m_destroyedLater = true;
}