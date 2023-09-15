#include "Engine.h"

Engine* Engine::s_engine = nullptr;
Vector Engine::s_gravity( { 0.0, 9.81f } );

Engine* Engine::getInstance()
{
    if( s_engine == nullptr )
    {
        s_engine = new Engine();
    }

    return s_engine;
}

void Engine::shootParticle( const float& initialAngle, const float& initialSpeed )
{
    float xVelocity = initialSpeed * cos( initialAngle );
    float yVelocity = initialSpeed * sin( initialAngle );

    Vector initialVelocity( { xVelocity, -yVelocity } );

    Particle newParticle( 1.0, initialVelocity, s_gravity, Vector( { 0.0, static_cast<float>(ofGetWindowHeight()), 0.0}));

    m_particles.push_back( newParticle );
}

void Engine::update( const float& time )
{
    for( Particle& currParticle : m_particles )
    {
        currParticle.update( time );
    }
}

void Engine::drawParticles() const
{
    for( const Particle& currParticle : m_particles )
    {
        currParticle.draw();
    }
}