#include "Engine.h"

Engine* Engine::s_engine = nullptr;
Vector Engine::s_gravity( { 0.0, 9.81f } );
float Engine::s_damping = 0.94;

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
    // l'axe y etant dirige vers le bas, il faut l'inverser afin de bien lancer la particule
    Vector initialVelocity( { xVelocity, -yVelocity } );

    Particle newParticle( 1.0, initialVelocity, s_gravity, Vector( { 0.0, static_cast< float >( ofGetWindowHeight() ), 0.0 } ) );

    m_particles.push_back( newParticle );
}

void Engine::update( const float& deltaTime )
{
    for( int i = 0; i < m_particles.size(); i++ )
    {
        m_particles.at( i ).update( deltaTime );
        // on suppriem les particules qui sont sorties en bas ou a droite
        if( m_particles.at( i ).getPosition().getX() > ofGetWindowWidth() + 5 || m_particles.at( i ).getPosition().getY() > ofGetWindowHeight() + 5 ) {
            m_particles.erase( m_particles.begin() + i );
            i--;
        }
    }
}

void Engine::drawParticles() const
{
    for( const Particle& currParticle : m_particles )
    {
        currParticle.draw();
    }
}