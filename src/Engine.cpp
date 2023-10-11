#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
int Engine::s_colorShift = 0;


/**
 * @brief R�cup�ration de l'instance du Singleton
 * @return 
*/
Engine* Engine::getInstance()
{
    if( s_engine == nullptr )
    {
        s_engine = new Engine();
    }

    return s_engine;
}


/**
 * @brief Tire une particule � une certaine position, avec un certain angle et une certaine pulsion
 * @param initialPos 
 * @param initialAngle 
 * @param initialSpeed 
 * @param mass 
 * @param radius 
 * @param color 
 * @param isFireball 
*/
void Engine::shootParticle( const Vector3& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass, const float& radius, const Vector3& color, bool isFireball )
{
    float xVelocity = initialSpeed * cos( initialAngle );
    float yVelocity = initialSpeed * sin( initialAngle );
    float zVelocity = 0;

    // l'axe y etant dirige vers le bas, il faut l'inverser afin de bien lancer la particule
    Vector3 initialVelocity( { xVelocity, -yVelocity, zVelocity } );

    // Id�alement il faudrait plut�t utiliser un design pattern comme une Factory si on pr�voit d'instancier plein de particules diff�rentes, �a serait plus extensible et facile � maintenir sur le long terme
    // Pour la phase 1, �a marche avec juste la boule de feu mais �a deviendra bien plus pertinent au fil du temps
    ParticlePtr newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = std::make_shared<Fireball>( mass, radius, initialVelocity, initialPos, color, s_colorShift );
    }
    else
    {
        newParticle = std::make_shared<Particle>( mass, radius, initialVelocity, initialPos, color );
    }

    m_particles.push_back( newParticle );
}

/**
 * @brief Mise � jour de la physique des particules en fonction du temps �coul� depuis la derni�re frame
 * @param deltaTime 
*/
void Engine::update( const float& deltaTime )
{
    // Ajout des forces au registre
    for( ParticlePtr& particle : m_particles )
    {
        // Gravit�
        m_particleForceRegistry.add( particle, std::make_shared<ParticleGravity>( m_gravity ) );
    }

    // Mise � jour des forces
    m_particleForceRegistry.updateForces( deltaTime );

    // Nettoyage du registre
    m_particleForceRegistry.clear();

    // Mise � jour physique de chaque particule
    for( ParticlePtr& particle : m_particles )
    {
        particle->integrate( deltaTime );
    }

    // Nettoyage des particules inutiles
    cleanup();
}


/**
 * @brief Supprime les particules sorties de l'�cran ou trop petites
*/
void Engine::cleanup()
{
    std::list<ParticlePtr>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // Si la particule est sortie de l'�cran ou est trop petite => Suppression
        if( ( ( *particleIterator )->getPosition().getX() > ofGetWindowWidth() + 5 || ( *particleIterator )->getPosition().getY() > ofGetWindowHeight() + 5 )
            || ( *particleIterator )->getRadius() <= 3.0 ) {
            particleIterator = m_particles.erase( particleIterator );
        }
        else
        {
            particleIterator++;
        }
    }
}


/**
 * @brief Dessine les particules
*/
void Engine::drawParticles() const
{
    for( const ParticlePtr& currParticle : m_particles )
    {
        currParticle->draw();
    }
}


/**
 * @brief Calcule une nouvelle valeur al�atoire pour un canal de couleur (R, G ou B) entre 0 et 255, sans d�passer un d�calage �gal � shiftAmount
 * @param colorChannel 
 * @param shiftAmount 
 * @return 
*/
float Engine::randshiftColorChannel( const float& colorChannel, const int& shiftAmount )
{
    float newColorChannel = colorChannel + rand() % ( shiftAmount * 2 + 1 ) - shiftAmount;

    if( newColorChannel < 0 ) newColorChannel = 0;
    if( newColorChannel > 255 ) newColorChannel = 255;

    return newColorChannel;
}


/**
 * @brief Calcule une nouvelle couleur RGB al�atoire � partir d'une couleur existante. Utilise la m�thode randshiftColorChannel.
 * @param color 
 * @param shiftAmount 
 * @return 
*/
Vector3 Engine::randshiftColor( const Vector3& color, const int& shiftAmount )
{
    Vector3 newColor( color );

    newColor.x = randshiftColorChannel( newColor.getX(), shiftAmount );
    newColor.y = randshiftColorChannel( newColor.getY(), shiftAmount );
    newColor.z = randshiftColorChannel( newColor.getZ(), shiftAmount );

    return newColor;
}


/**
 * @brief G�re le comportement des particules face � un clic de souris.
 * @param x 
 * @param y 
 * @return 
*/
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
        if( ( *particleIterator )->toBeDestroyed() )
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