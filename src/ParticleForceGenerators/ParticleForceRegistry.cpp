#include "ParticleForceRegistry.h"

/**
 * @brief Ajout d'une association particule/force dans le registre
 * @param particle 
 * @param forceGenerator 
*/
void ParticleForceRegistry::add( std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator )
{
    m_registry.push_back( ParticleForceRegistration( { particle, forceGenerator } ) );
}

/**
 * @brief Retrait d'une association particule/force du registre
 * @param particle 
 * @param forceGenerator 
*/
void ParticleForceRegistry::remove( std::shared_ptr<Particle> particle, std::shared_ptr<ParticleForceGenerator> forceGenerator )
{
    Registry::iterator registryIterator;
    bool registrationFound = false;

    // Pour chaque registration
    for( registryIterator = m_registry.begin(); registryIterator != m_registry.end() && !registrationFound; registryIterator++ )
    {
        // Si l'association a été trouvée
        if( ( *registryIterator ).particle == particle && ( *registryIterator ).forceGenerator == forceGenerator )
        {
            // On la supprime
            registrationFound = true;
            m_registry.erase( registryIterator );
        }
    }
}

/**
 * @brief Suppression du registre (mais pas des forces/générateurs qui y étaient contenus)
*/
void ParticleForceRegistry::clear()
{
    m_registry.clear();
}


/**
 * @brief Pour chaque élément du registre, met à jour la particule courante à l'aide du générateur associé
 * @param duration 
*/
void ParticleForceRegistry::updateForces( const float& duration )
{
    for( ParticleForceRegistration& registration : m_registry )
    {
        registration.forceGenerator->updateForce( registration.particle, duration );
    }
}
