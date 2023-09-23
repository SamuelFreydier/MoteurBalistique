#include "Engine.h"

Engine* Engine::s_engine = nullptr;
Vector Engine::s_gravity( { 0.0, 9.81f } );
float Engine::s_damping = 0.94;
int Engine::s_colorShift = 0;

Engine* Engine::getInstance()
{
    if( s_engine == nullptr )
    {
        s_engine = new Engine();
    }

    return s_engine;
}

void Engine::shootParticle( const Vector& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass, const float& radius, bool hasTrail, const Vector& color, bool isFireball )
{
    float xVelocity = initialSpeed * cos( initialAngle );
    float yVelocity = initialSpeed * sin( initialAngle );
    // l'axe y etant dirige vers le bas, il faut l'inverser afin de bien lancer la particule
    Vector initialVelocity( { xVelocity, -yVelocity } );

    // Idéalement il faudrait plutôt utiliser un design pattern comme une Factory si on prévoit d'instancier plein de particules différentes, ça serait plus extensible et facile à maintenir sur le long terme
    // Pour ce TP je voulais juste faire ma boule de feu parce que c'est le fun, fait que j'm'en balance
    ParticlePtr newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = std::make_shared<Fireball>( mass, radius, hasTrail, initialVelocity, s_gravity, initialPos, color, s_colorShift );
    }
    else
    {
        newParticle = std::make_shared<Particle>( mass, radius, hasTrail, initialVelocity, s_gravity, initialPos, color );
    }

    m_particles.push_back( newParticle );
}


void Engine::update( const float& deltaTime )
{
    updateParticleList( m_particles, deltaTime );
    updateParticleList( m_vanillaParticles, deltaTime );
}

void Engine::updateParticleList( std::list<ParticlePtr>& particleList, const float& deltaTime )
{
    std::list<ParticlePtr>::iterator particleIterator = particleList.begin();

    while( particleIterator != particleList.end() )
    {
        ( *particleIterator )->update( deltaTime );
        // on supprime les particules qui sont sorties en bas ou a droite
        if( ( ( *particleIterator )->getPosition().getX() > ofGetWindowWidth() + 5 || ( *particleIterator )->getPosition().getY() > ofGetWindowHeight() + 5 )
            || ( *particleIterator )->getRadius() <= 3.0 ) {
            particleIterator = particleList.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }
    }
}

void Engine::drawParticles() const
{
    for( const ParticlePtr& currParticle : m_particles )
    {
        currParticle->draw();
    }

    for( const ParticlePtr& currVanillaParticle : m_vanillaParticles )
    {
        currVanillaParticle->draw();
    }
}

float Engine::randshiftColorChannel( const float& colorChannel, const int& shiftAmount )
{
    float newColorChannel = colorChannel + rand() % ( shiftAmount * 2 + 1 ) - shiftAmount;

    if( newColorChannel < 0 ) newColorChannel = 0;
    if( newColorChannel > 255 ) newColorChannel = 255;

    return newColorChannel;
}

Vector Engine::randshiftColor( const Vector& color, const int& shiftAmount )
{
    Vector newColor( color );

    for( int channelIdx = 0; channelIdx < newColor.getDimension(); channelIdx++ )
    {
        newColor[ channelIdx ] = randshiftColorChannel( newColor[ channelIdx ], shiftAmount );
    }

    return newColor;
}

bool Engine::clickedParticle( const float& x, const float& y )
{
    bool clicked = false;
    std::list<ParticlePtr>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() && clicked == false )
    {
        if( ( (*particleIterator)->getPosition().getX() - ( *particleIterator )->getRadius() < x && ( *particleIterator )->getPosition().getX() + ( *particleIterator )->getRadius() > x )
            && ( ( *particleIterator )->getPosition().getY() - ( *particleIterator )->getRadius() < y && ( *particleIterator )->getPosition().getY() + ( *particleIterator )->getRadius() > y ) )
        {
            clicked = true;
            ( *particleIterator )->clicked();
        }
        if( ( *particleIterator )->IsDead() )
        {
            particleIterator = m_particles.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }
    }

    return clicked;
}