#include "Fireball.h"
#include "Engine.h"

Fireball::Fireball( const float& mass, const float& radius, const Vector3& velocity, const Vector3& position, const Vector3& color, const bool& showParticleInfos, const int& colorShift )
    : Particle( mass, radius, velocity, position, color, showParticleInfos), m_colorShift( colorShift )
{

}

Fireball::Fireball( const Fireball& fireball )
    : Particle( fireball ), m_colorShift( fireball.getColorShift() )
{

}


/**
 * @brief Comportement normal de particule + cr�ation de particules pour g�n�rer la train�e de la boule de feu
 * @param deltaTime 
*/
void Fireball::integrate( const float& deltaTime )
{
    const Vector3 lastPosition( getPosition() );

    // Comportement normal de Particule
    Particle::integrate( deltaTime );

    const int nbAshfalls = rand() % 2 + 3; // Entre 3 et 4 retomb�es de cendres


    const float ashFallRadius = getRadius() * 0.3;

    // Calcul de la position d'apparition    
    const float traveledDistance = lastPosition.distance(getPosition());
    const Vector3 positionShift = (getPosition() - lastPosition) * (1 - m_radius / traveledDistance);

    // Pour chaque particule de cendre (composant la train�e)
    for( int ashfallIdx = 0; ashfallIdx < nbAshfalls; ashfallIdx++ )
    {
        // Calcul de sa couleur (proche de celle de la boule de feu)
        Vector3 colorShift( Engine::randshiftColor( m_color, m_colorShift ) );

        // newPosition = le point situ� � l'intersection entre la p�riph�rie du cercle de la boule de feu � l'instant t et la droite reliant la boule � t-1 et la boule � t
        Vector3 newPosition( lastPosition + positionShift );

        // La position est al�atoire � + ou - pi/2 de d�calage par rapport � newPosition (la particule va appara�tre al�atoirement sur un arc de cercle dont le milieu est newPosition)
        float angle = atan2( getPosition().getY() - newPosition.getY(), getPosition().getX() - newPosition.getX() ) + PI;
        angle +=  ( rand() % ( int ) ( PI * 100 + 1 ) ) / 100.0 - PI / 2;
        newPosition.x += cos(angle) * (m_radius ); 
        newPosition.y += sin( angle ) * (m_radius);

        // Cr�ation de la particule (plus petite que la boule de feu d'origine)
        Particle* ashfall = new Particle( 0.0001, getRadius() * 0.3, Vector3(), newPosition, colorShift);

        // Disparition progressive de la train�e de la boule de feu
        ashfall->setSizeModificator( ( rand() % 8 + 85 ) / 100.0 );
        Engine::getInstance()->addTempAshFallParticles( ashfall ); // Pour l'instant on les met dans une liste temporaire pour �viter de modifier Engine::m_particles (bug d'it�rateur dans Engine::RunPhysics() sinon)
    }
}


/**
 * @brief Fait exploser la boule de feu � la d�tection du clic
*/
void Fireball::clicked()
{
    explode();
}


/**
 * @brief Explosion de la boule de feu en n plus petites boules de feu partant dans toutes les directions
*/
void Fireball::explode()
{
    float angleStep = PI * 2 / m_nbExplosionProjectiles;

    // Une it�ration = un d�bris qui part dans une direction
    for( float angleProjection = 0.0; angleProjection < 2 * PI; angleProjection += angleStep )
    {
        const Vector3 initialVelocity = getVelocity() + Vector3({ getVelocity().norm() * cos(angleProjection), getVelocity().norm() * sin(angleProjection), 0.0 }) * 2;
        Engine::getInstance()->shootParticle( getPosition(), initialVelocity, getMass() / m_nbExplosionProjectiles, getRadius() * pow(m_nbExplosionProjectiles, -0.3333333), m_color, true); // le pow c'est pour trouver le rayon de sous-boules provenant de la fragmentation d'une boule tout en conservant le volume
    }

    // On laisse la responsabilit� � l'Engine de d�truire la boule
    m_destroyedLater = true;
}