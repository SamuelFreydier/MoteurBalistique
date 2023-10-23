#include "Engine.h"

Engine* Engine::s_engine = nullptr;
float Engine::s_damping = 0.94;
bool Engine::s_realisticAirResistance = false;
int Engine::s_colorShift = 0;
Referential Engine::s_referential = Referential();


/**
 * @brief Récupération de l'instance du Singleton
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
 * @brief Tire une particule à une certaine position, avec un certain angle et une certaine pulsion
 * @param initialPos 
 * @param initialAngle 
 * @param initialSpeed 
 * @param mass 
 * @param radius 
 * @param color 
 * @param isFireball 
*/
void Engine::shootParticle(const Vector3& initialPos, const Vector3& initialVelocity , const float& mass, const float& radius, const Vector3& color, bool isFireball, bool m_showParticleInfos)
{
    // Idéalement il faudrait plutôt utiliser un design pattern comme une Factory si on prévoit d'instancier plein de particules différentes, ça serait plus extensible et facile à maintenir sur le long terme
    // Pour la phase 1, ça marche avec juste la boule de feu mais ça deviendra bien plus pertinent au fil du temps
    ParticlePtr newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = std::make_shared<Fireball>( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos, s_colorShift );
    }
    else
    {
        newParticle = std::make_shared<Particle>( mass, radius, initialVelocity, initialPos, color, m_showParticleInfos);
    }

    m_particles.push_back( newParticle );
}

/**
 * @brief Mise à jour de la physique des particules en fonction du temps écoulé depuis la dernière frame
 * @param deltaTime 
*/
void Engine::update( const float& secondsElapsedSincePreviousUpdate)
{
    // Ajout des forces au registre
    for( ParticlePtr& particle : m_particles )
    {
        // Gravité
        m_particleForceRegistry.add( particle, std::make_shared<ParticleGravity>( m_gravity ) );
    }

    // Mise à jour des forces
    m_particleForceRegistry.updateForces(secondsElapsedSincePreviousUpdate);

    // Nettoyage du registre
    m_particleForceRegistry.clear();

    // Mise à jour physique de chaque particule
    for( ParticlePtr& particle : m_particles )
    {
        particle->update(secondsElapsedSincePreviousUpdate);
    }

    // Nettoyage des particules inutiles
    cleanup();
}


/**
 * @brief Supprime les particules sorties de l'écran ou trop petites
*/
void Engine::cleanup()
{
    std::list<ParticlePtr>::iterator particleIterator = m_particles.begin();

    while( particleIterator != m_particles.end() )
    {
        // on supprime les particules qui sont sorties par le bas et celles qui sont devenues trop petites (trainées de cendres)
        if( ( ( *particleIterator )->getPosition().getY() - (*particleIterator)->getRadius() < 0 )
            || ( *particleIterator )->getRadius() < 0.009 ) {
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
 * @brief Calcule une nouvelle valeur aléatoire pour un canal de couleur (R, G ou B) entre 0 et 255, sans dépasser un décalage égal à shiftAmount
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
 * @brief Calcule une nouvelle couleur RGB aléatoire à partir d'une couleur existante. Utilise la méthode randshiftColorChannel.
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
 * @brief Gère le comportement des particules face à un clic de souris.
 * @param x 
 * @param y 
 * @return 
*/
bool Engine::clickedParticle( const float& x, const float& y )
{
    bool clicked = false;


    const bool conversionIsFromGraphicToMecanic = false;
    const Vector3 clicGraphique = Vector3({ x, y, 0.0 });
    const Vector3 clicMecanique = s_referential.conversionPositionMecaniqueGraphique(clicGraphique, conversionIsFromGraphicToMecanic);

    const float clicMecaniqueX = clicMecanique.getX();
    const float clicMecaniqueY = clicMecanique.getY();


    std::list<ParticlePtr>::iterator particleIterator = m_particles.begin();
    while( particleIterator != m_particles.end() && clicked == false )
    {
        const float particlePositionX = (*particleIterator)->getPosition().getX();
        const float particlePositionY = (*particleIterator)->getPosition().getY();
        const float particleRadius = (*particleIterator)->getRadius();

        if( (particlePositionX - particleRadius < clicMecaniqueX && particlePositionX + particleRadius > clicMecaniqueX)
            && (particlePositionY - particleRadius < clicMecaniqueY && particlePositionY + particleRadius > clicMecaniqueY) )
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


void Engine::showScore(const bool& boolShowScore) const
{
    if (boolShowScore)
    {
        ofSetColor(255, 100, 100);
        ofDrawBitmapString("Score : " + ofToString(score), (Vector3({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 })).v3());
    }
}