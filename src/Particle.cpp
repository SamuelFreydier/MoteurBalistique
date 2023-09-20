#include "Particle.h"

Particle::Particle( const float& mass, const Vector& velocity, const Vector& acceleration, const Vector& position )
    : m_massReverse( 1 / mass ), m_initialVelocity( velocity ), m_velocity( velocity ), m_acceleration( acceleration ), m_position( position )
{

}

void Particle::update( const float& deltaTime )
{
    m_velocity[ 0 ] += deltaTime * m_acceleration.getX();
    m_velocity[ 1 ] += deltaTime * m_acceleration.getY();

    m_position[0] += deltaTime * m_velocity.getX();
    m_position[1] += deltaTime * m_velocity.getY();
}

void Particle::draw() const
{
    ofSpherePrimitive graphicParticle;

    graphicParticle.setPosition( m_position.v3() );

    graphicParticle.draw();
}