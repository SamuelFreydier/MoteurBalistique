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
 * @brief Calcul de la nouvelle v�locit� et de la nouvelle position � partir de l'acc�l�ration via l'int�gration d'Euler + Modification de la taille.
 * @param deltaTime 
*/
void Particle::update( const float& secondsElapsedSincePreviousUpdate)
{
    if (Engine::getRealisticAirResistance()) // Si on applique un mod�le r�aliste, alors principe fondamental de la dynamique + formule force de train�e
    {
        float frontalSurface = PI * pow(m_radius, 2); // surface frontale d'une sph�re
        float normeForceTrainee = constTrainee * frontalSurface * pow(m_velocity.norm(), 2); // air immobile donc la vitesse relative par rapport � la masse d'air est juste la vitesse de la particule
        
        Vector forceTrainee = Vector({ 0.0, 0.0, 0.0 });
        if (m_velocity.norm() != 0)
        {
            forceTrainee = m_velocity * (1 / m_velocity.norm()) * normeForceTrainee * -1; // on multiplie par le vecteur v�locit� normalis� et on prend le sens inverse
        }
       
        //float forceTraineeX = -constTrainee * frontalSurface * m_velocity.getX() * abs(m_velocity.getX()); // le - et le abs() servent � orienter correctement le vecteur r�sultant de la force de Trainee
        //float forceTraineeY = -constTrainee * frontalSurface * m_velocity.getY() * abs(m_velocity.getY());

        if ((forceTrainee * m_massReverse * secondsElapsedSincePreviousUpdate).norm() >= m_velocity.norm()) // bug qui reste de d�clencher des glitch, particule qui part dans la mauvaise direction ou se t�l�porte � cause d'une trop grande force de train�e
        {
            m_velocity = Vector({ 0.0, 0.0, 0.0 });
            m_velocity += Engine::getGravity() * secondsElapsedSincePreviousUpdate;
        }
        else // si tout va bien 
        {
            // On fait le bilan des forces avec l'acceleration terrestre qui est l� dans tous les cas
            Vector acceleration = (forceTrainee * m_massReverse) + Engine::getGravity();  

            // update velocity
            m_velocity += acceleration * secondsElapsedSincePreviousUpdate;
        }
    }
    else // sinon, on utilise le damping qui est un param�tre reglable
    {
        m_velocity *= pow(Engine::getInstance()->getDamping(), secondsElapsedSincePreviousUpdate); // application frottements de l'air pas r�alistes
        m_velocity += Engine::getGravity() * secondsElapsedSincePreviousUpdate; // l'acceleration terrestre qui est l� dans tous les cas
    }
    

    // Position
    m_position += m_velocity * secondsElapsedSincePreviousUpdate;

    // Taille
    m_radius *= m_sizeModificator;
}


/**
 * @brief Dessine la particule sous forme de sph�re
*/
void Particle::draw() const
{
    // version Samuel obsol�te avec graphicParticle
    /*
    ofSpherePrimitive graphicParticle;

    graphicParticle.setPosition( m_position.v3() );
    graphicParticle.setRadius( m_radius );

    ofSetColor( m_color.getX(), m_color.getY(), m_color.getZ() );
    
    graphicParticle.draw();
    
    */



    // Version Ana�l avec ofDrawparticle() et manipulations du r�f�rentiel
    // 
    // transformations successives du r�f�rentiel m�canique pour le placer dans le r�f�rentiel graphique
    bool isConversionFromMecamicToGraphic = true;
    Vector positionGraphique = Engine::getInstance()->getReferential().conversionPositionMecaniqueGraphique(m_position, isConversionFromMecamicToGraphic);

    float scaleReferential = Engine::getInstance()->getReferential().getScale();

    ofSetColor(m_color.getX(), m_color.getY(), m_color.getZ());
    ofDrawCircle(positionGraphique.v3(), m_radius / scaleReferential); // On dessine sans oublier d'�galement mettre la taille de la particule � l'�chelle

    if (m_showParticleInfos)
    {
        // calcul de la vitesse en m/s pour affichage en temps r�el;
        float vitesseParticule = m_velocity.norm();
        ofDrawBitmapString(ofToString(vitesseParticule) + " m/s\n" + ofToString(1 / m_massReverse) + " kg", (positionGraphique + Vector({0.0, - 20 - m_radius / scaleReferential, 0.0})).v3());
    }
}