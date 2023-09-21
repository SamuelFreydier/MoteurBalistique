#include "Particle.h"
#include "Engine.h"

Particle::Particle( const float& mass, bool hasTrail, const Vector& velocity, const Vector& acceleration, const Vector& position, const Vector& color )
    : m_massReverse( 1 / mass ), m_hasTrail( hasTrail ), m_velocity( velocity ), m_acceleration( acceleration ), m_position( position ), m_color( color )
{

}

Particle::Particle( const Particle& particle )
    : Particle(particle.getMass(), particle.hasTrail(), particle.getVelocity(), particle.getAcceleration(), particle.getPosition(), particle.getColor() )
{

}

void Particle::update( const float& deltaTime )
{
    m_velocity[ 0 ] = m_velocity[ 0 ] * pow( Engine::getInstance()->getDamping(), deltaTime ) + deltaTime * m_acceleration.getX();
    m_velocity[ 1 ] = m_velocity[ 1 ] * pow( Engine::getInstance()->getDamping(), deltaTime ) + deltaTime * m_acceleration.getY();

    m_position[ 0 ] += deltaTime * m_velocity.getX();
    m_position[ 1 ] += deltaTime * m_velocity.getY();
}

void Particle::draw() const
{
    ofSpherePrimitive graphicParticle;

    graphicParticle.setPosition( m_position.v3() );

    ofSetColor( m_color.getX(), m_color.getY(), m_color.getZ() );
    
    graphicParticle.draw();
}