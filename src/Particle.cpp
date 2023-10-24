#include "Particle.h"
#include "Engine.h"

Particle::Particle( const float& mass, const float& radius, const Vector3& velocity, const Vector3& position, const Vector3& color, const bool& showParticleInfos)
    : m_inverseMass( 1 / mass ), m_radius( radius ), m_velocity( velocity ), m_position( position ), m_color( color ), m_showParticleInfos(showParticleInfos)
{

}

Particle::Particle( const Particle& particle )
    : Particle(particle.getMass(), particle.getRadius(), particle.getVelocity(), particle.getPosition(), particle.getColor(), particle.getShowParticleInfos())
{
    m_acceleration = particle.m_acceleration;
    m_accumForce = particle.m_accumForce;
}


/**
 * @brief Ajoute la force forceVector au vecteur d'accumulation m_accumForce
 * @param forceVector
*/
void Particle::addForce( const Vector3& forceVector, const bool& isFrictionGlitch )
{
    if (isFrictionGlitch)
    {
        m_accumForce.glitchFriction = true;
    }
    else
    {
        m_accumForce.accumForce += forceVector;
    }
}

/**
 * @brief Nettoie le vecteur d'accumulation m_accumForce (=> vecteur nul)
*/
void Particle::clearAccum()
{
    m_accumForce.accumForce.clear();
    m_accumForce.glitchFriction = false;
}


/**
 * @brief Calcul de la nouvelle vélocité et de la nouvelle position à partir de l'accélération via l'intégration d'Euler + Modification de la taille.
 * @param deltaTime
*/
void Particle::integrate( const float& secondsElapsedSincePreviousUpdate)
{
    if (m_accumForce.glitchFriction == true) // si il y a eu un glitch de friction, alors on annule la vélocité de la particule pour éviter des comportements illégaux (téléportations, partir dans le mauvais sens)
    {
        m_velocity = Vector3(); 
    }

    if (Engine::getRealisticAirResistance() == true) // Choix de l'utilisateur entre frottements de l'air réalistes ou pas
    {
        m_velocity *= pow(Engine::getInstance()->getDamping(), secondsElapsedSincePreviousUpdate); // on applique
    }

    // Maintenant on peut faire comme d'habitude avec l'intégrateur d'Euler :

    // Accélération
    m_acceleration = m_accumForce.accumForce / getMass();
          
    // Vélocité
    m_velocity += m_acceleration * secondsElapsedSincePreviousUpdate;
            
    // Position
    m_position += m_velocity * secondsElapsedSincePreviousUpdate;

    // Taille
    m_radius *= m_sizeModificator;

    // Nettoyage du vecteur d'accumulation de forces
    clearAccum();
}


/**
 * @brief Dessine la particule sous forme de sphère
*/
void Particle::draw() const
{
    // version Samuel obsolète avec graphicParticle
    /*
    ofSpherePrimitive graphicParticle;

    graphicParticle.setPosition( m_position.v3() );
    graphicParticle.setRadius( m_radius );

    ofSetColor( m_color.getX(), m_color.getY(), m_color.getZ() );
    
    graphicParticle.draw();
    
    */



    // Version Anaël avec ofDrawparticle() et manipulations du référentiel
    // 
    // transformations successives du référentiel mécanique pour le placer dans le référentiel graphique
    bool isConversionFromMecamicToGraphic = true;
    Vector3 positionGraphique = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(m_position, isConversionFromMecamicToGraphic);

    float scaleReferential = Engine::getInstance()->getReferential().getScale();

    ofSetColor(m_color.getX(), m_color.getY(), m_color.getZ());
    ofDrawCircle(positionGraphique.v3(), m_radius / scaleReferential); // On dessine sans oublier d'également mettre la taille de la particule à l'échelle

    if (m_showParticleInfos)
    {
        // calcul de la vitesse en m/s pour affichage en temps réel;
        float vitesseParticule = m_velocity.norm();
        ofDrawBitmapString(ofToString(vitesseParticule) + " m/s\n" + ofToString(1 / m_inverseMass) + " kg", (positionGraphique + Vector3({0.0, - 20 - m_radius / scaleReferential, 0.0})).v3());
    }
}