#include "Particle.h"
#include "Engine.h"

Particle::Particle( const float& mass, const float& radius, const Vector& velocity, const Vector& acceleration, const Vector& position, const Vector& color )
    : m_massReverse( 1 / mass ), m_radius( radius ), m_velocity( velocity ), m_acceleration( acceleration ), m_position( position ), m_color( color ), m_accumForce( Vector( DEFAULT_VCT_DIMENSION ) )
{

}

Particle::Particle( const Particle& particle )
    : Particle(particle.getMass(), particle.getRadius(), particle.getVelocity(), particle.getAcceleration(), particle.getPosition(), particle.getColor() )
{
    m_accumForce = particle.m_accumForce;
}

/**
 * @brief Ajoute la force forceVector au vecteur d'accumulation m_accumForce
 * @param forceVector 
*/
void Particle::addForce( const Vector& forceVector )
{
    m_accumForce += forceVector;
}

/**
 * @brief Nettoie le vecteur d'accumulation m_accumForce (=> vecteur nul)
*/
void Particle::clearAccum()
{
    m_accumForce.clear();
}


/**
 * @brief Calcul de la nouvelle vélocité et de la nouvelle position à partir de l'accélération via l'intégration d'Euler + Modification de la taille.
 * @param deltaTime
*/
void Particle::update( const float& deltaTime )
{
    // Vélocité
    m_velocity = m_velocity * pow(Engine::getInstance()->getDamping(), deltaTime) + m_acceleration * deltaTime;
    
    // Position
    m_position += m_velocity * deltaTime; 

    // Taille
    m_radius *= m_sizeModificator;
}


/**
 * @brief Dessine la particule sous forme de sphère
*/
void Particle::draw() const
{
    ofSpherePrimitive graphicParticle;

    graphicParticle.setPosition( m_position.v3() );
    graphicParticle.setRadius( m_radius );

    ofSetColor( m_color.getX(), m_color.getY(), m_color.getZ() );
    
    graphicParticle.draw();
}