#include "Rigidbody.h"
#include "Engine.h"

Rigidbody::Rigidbody( const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& color )
    : m_velocity( velocity ), m_position( position ), m_color( color )
{
    setMassReverse(mass);
}

Rigidbody::Rigidbody( const Rigidbody& particle )
    : Rigidbody(particle.getMass(), particle.getVelocity(), particle.getPosition(), particle.getColor() )
{
    m_acceleration = particle.m_acceleration;
    m_accumForce = particle.m_accumForce;
}

void Rigidbody::setMassReverse(const float& mass)
{
    if (mass == 0) 
    {
        m_inverseMass = std::numeric_limits<float>::max();
    }
    else
    {
        m_inverseMass = 1 / mass;
    }
}


/**
 * @brief Ajoute la force forceVector au vecteur d'accumulation m_accumForce
 * @param forceVector
*/
void Rigidbody::addForce( const Vector3& forceVector )
{
    m_accumForce += forceVector;

}

/**
 * @brief Nettoie le vecteur d'accumulation m_accumForce (=> vecteur nul)
*/
void Rigidbody::clearAccum()
{
    m_accumForce.clear();
}


/**
 * @brief Calcul de la nouvelle vélocité et de la nouvelle position à partir de l'accélération via l'intégration d'Euler + Modification de la taille.
 * @param deltaTime
*/
void Rigidbody::integrate( const float& secondsElapsedSincePreviousUpdate)
{
    // Accélération
    m_acceleration = m_accumForce * getInverseMass();
          
    // Vélocité
    m_velocity = m_velocity * pow( Engine::getInstance()->getDamping(), secondsElapsedSincePreviousUpdate ) + m_acceleration * secondsElapsedSincePreviousUpdate;
            
    // Position
    m_position += m_velocity * secondsElapsedSincePreviousUpdate;

    // Nettoyage du vecteur d'accumulation de forces
    clearAccum();
}


/**
 * @brief Dessine la particule sous forme de sphère
*/
void Rigidbody::draw() const
{
    // Version oskour qu'est-ce que je fais
    ofColor color(m_color.x, m_color.y, m_color.z);
    ofSetColor(color);
    //ofDrawSphere(m_position.v3(), m_radius);
}