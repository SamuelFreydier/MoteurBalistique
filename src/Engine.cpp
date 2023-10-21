#include "Engine.h"

Engine* Engine::s_engine = nullptr;
Vector Engine::s_gravity( { 0.0, - 9.81f, 0.0 } );
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
void Engine::shootParticle(const Vector& initialPos, const Vector& initialVelocity , const float& mass, const float& radius, const Vector& color, bool isFireball, bool m_showParticleInfos)
{
    // Idéalement il faudrait plutôt utiliser un design pattern comme une Factory si on prévoit d'instancier plein de particules différentes, ça serait plus extensible et facile à maintenir sur le long terme
    // Pour la phase 1, ça marche avec juste la boule de feu mais ça deviendra bien plus pertinent au fil du temps
    ParticlePtr newParticle = nullptr;
    if( isFireball == true )
    {
        newParticle = std::make_shared<Fireball>( mass, radius, initialVelocity, s_gravity, initialPos, color, m_showParticleInfos, s_colorShift );
    }
    else
    {
        newParticle = std::make_shared<Particle>( mass, radius, initialVelocity, s_gravity, initialPos, color, m_showParticleInfos);
    }

    m_particles.push_back( newParticle );
}

/**
 * @brief Mise à jour de la physique des particules en fonction du temps écoulé depuis la dernière frame
 * @param deltaTime 
*/
void Engine::update( const float& deltaTime )
{
    updateParticleList( m_particles, deltaTime );
    updateParticleList( m_vanillaParticles, deltaTime );
}


/**
 * @brief Mise à jour de la physique des particules D'UNE LISTE PRECISE
 * @param particleList 
 * @param deltaTime 
*/
void Engine::updateParticleList( std::list<ParticlePtr>& particleList, const float& deltaTime )
{
    std::list<ParticlePtr>::iterator particleIterator = particleList.begin();

    while( particleIterator != particleList.end() )
    {
        ( *particleIterator )->update( deltaTime );
        // on supprime les particules qui sont sorties par le bas et celles qui sont devenues trop petites (trainées de cendres)
        if( ( ( *particleIterator )->getPosition().getY() - (*particleIterator)->getRadius() < 0 )
            || ( *particleIterator )->getRadius() < 0.009 ) {
            particleIterator = particleList.erase( particleIterator );
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

    for( const ParticlePtr& currVanillaParticle : m_vanillaParticles )
    {
        currVanillaParticle->draw();
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
Vector Engine::randshiftColor( const Vector& color, const int& shiftAmount )
{
    Vector newColor( color );

    for( int channelIdx = 0; channelIdx < newColor.getDimension(); channelIdx++ )
    {
        newColor[ channelIdx ] = randshiftColorChannel( newColor[ channelIdx ], shiftAmount );
    }

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
    const Vector clicGraphique = Vector({ x, y, 0.0 });
    const Vector clicMecanique = s_referential.conversionPositionMecaniqueGraphique(clicGraphique, conversionIsFromGraphicToMecanic);

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
        if( ( *particleIterator )->ToBeDestroyed() )
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
        ofDrawBitmapString("Score : " + ofToString(score), (Vector({ (float)ofGetMouseX(), (float)ofGetMouseY(), 0.0 })).v3());
    }
}