#include "Particle.h"
#include "Engine.h"

Particle::Particle( const float& mass, const float& radius, const Vector& velocity, const Vector& acceleration, const Vector& position, const Vector& color, const bool& showParticleInfos )
    : m_massReverse( 1 / mass ), m_radius( radius ), m_velocity( velocity ), m_acceleration( acceleration ), m_position( position ), m_color( color ), m_showParticleInfos(showParticleInfos)
{

}

Particle::Particle( const Particle& particle )
    : Particle(particle.getMass(), particle.getRadius(), particle.getVelocity(), particle.getAcceleration(), particle.getPosition(), particle.getColor(), particle.getShowParticleInfos())
{

}

/**
 * @brief Calcul de la nouvelle vélocité et de la nouvelle position à partir de l'accélération via l'intégration d'Euler + Modification de la taille.
 * @param deltaTime 
*/
void Particle::update( const float& secondsElapsedSincePreviousUpdate)
{
    if (Engine::getRealisticAirResistance()) // Si on applique un modèle réaliste, alors principe fondamental de la dynamique + formule force de trainée
    {
        float frontalSurface = PI * pow(m_radius, 2); // surface frontale d'une sphère
        float normeForceTrainee = constTrainee * frontalSurface * pow(m_velocity.norm(), 2); // air immobile donc la vitesse relative par rapport à la masse d'air est juste la vitesse de la particule
        
        Vector forceTrainee = Vector({ 0.0, 0.0, 0.0 });
        if (m_velocity.norm() != 0)
        {
            forceTrainee = m_velocity * (1 / m_velocity.norm()) * normeForceTrainee * -1; // on multiplie par le vecteur vélocité normalisé et on prend le sens inverse
        }
       
        //float forceTraineeX = -constTrainee * frontalSurface * m_velocity.getX() * abs(m_velocity.getX()); // le - et le abs() servent à orienter correctement le vecteur résultant de la force de Trainee
        //float forceTraineeY = -constTrainee * frontalSurface * m_velocity.getY() * abs(m_velocity.getY());

        if ((forceTrainee * m_massReverse * secondsElapsedSincePreviousUpdate).norm() >= m_velocity.norm()) // bug qui reste de déclencher des glitch, particule qui part dans la mauvaise direction ou se téléporte à cause d'une trop grande force de trainée
        {
            m_velocity = Vector({ 0.0, 0.0, 0.0 });
            m_velocity += Engine::getGravity() * secondsElapsedSincePreviousUpdate;
        }
        else // si tout va bien 
        {
            // On fait le bilan des forces avec l'acceleration terrestre qui est là dans tous les cas
            Vector acceleration = (forceTrainee * m_massReverse) + Engine::getGravity();  

            // update velocity
            m_velocity += acceleration * secondsElapsedSincePreviousUpdate;
        }
    }
    else // sinon, on utilise le damping qui est un paramètre reglable
    {
        m_velocity *= pow(Engine::getInstance()->getDamping(), secondsElapsedSincePreviousUpdate); // application frottements de l'air pas réalistes
        m_velocity += Engine::getGravity() * secondsElapsedSincePreviousUpdate; // l'acceleration terrestre qui est là dans tous les cas
    }
    

    // Position
    m_position += m_velocity * secondsElapsedSincePreviousUpdate;

    // Taille
    m_radius *= m_sizeModificator;
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
    Vector positionGraphique = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(m_position, isConversionFromMecamicToGraphic);

    float scaleReferential = Engine::getInstance()->getReferential().getScale();

    ofSetColor(m_color.getX(), m_color.getY(), m_color.getZ());
    ofDrawCircle(positionGraphique.v3(), m_radius / scaleReferential); // On dessine sans oublier d'également mettre la taille de la particule à l'échelle

    if (m_showParticleInfos)
    {
        // calcul de la vitesse en m/s pour affichage en temps réel;
        float vitesseParticule = m_velocity.norm();
        ofDrawBitmapString(ofToString(vitesseParticule) + " m/s\n" + ofToString(1 / m_massReverse) + " kg", (positionGraphique + Vector({0.0, - 20 - m_radius / scaleReferential, 0.0})).v3());
    }
}