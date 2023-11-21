#include "ParticleForceRegistry.h"

/**
 * @brief Ajout d'une association particule/force dans le registre
 * @param particle 
 * @param forceGenerator 
*/
void ForceRegistry::add( std::shared_ptr<Particle> particle, std::shared_ptr<ForceGenerator> forceGenerator )
{
    m_particleRegistry.push_back( ParticleForceRegistration( { particle, forceGenerator } ) );
}

/**
 * @brief Retrait d'une association particule/force du registre
 * @param particle 
 * @param forceGenerator 
*/
void ForceRegistry::remove( std::shared_ptr<Particle> particle, std::shared_ptr<ForceGenerator> forceGenerator )
{
    ParticleRegistry::iterator registryIterator;
    bool registrationFound = false;

    // Pour chaque registration
    for( registryIterator = m_particleRegistry.begin(); registryIterator != m_particleRegistry.end() && !registrationFound; registryIterator++ )
    {
        // Si l'association a été trouvée
        if( ( *registryIterator ).particle == particle && ( *registryIterator ).forceGenerator == forceGenerator )
        {
            // On la supprime
            registrationFound = true;
            m_particleRegistry.erase( registryIterator );
        }
    }
}

/**
 * @brief Retrait de toutes les associations liées à la particule passée en paramètre
 * @param particle 
*/
void ForceRegistry::remove( std::shared_ptr<Particle> particle )
{
    ParticleRegistry::iterator registryIterator = m_particleRegistry.begin();

    // Parcours de toutes les associations
    while( registryIterator != m_particleRegistry.end() )
    {
        // Si l'association a été trouvée
        if( ( *registryIterator ).particle == particle )
        {
            // On la supprime
            registryIterator = m_particleRegistry.erase( registryIterator );
        }
        else
        {
            registryIterator++;
        }
    }
}


/**
 * @brief Ajout d'une association rigidbody/force dans le registre
 * @param rigidbody
 * @param forceGenerator
*/
void ForceRegistry::add(std::shared_ptr<Rigidbody> rigidbody, std::shared_ptr<ForceGenerator> forceGenerator)
{
    m_rigidbodyRegistry.push_back(RigidbodyForceRegistration({ rigidbody, forceGenerator }));
}

/**
 * @brief Retrait d'une association rigidbody/force du registre
 * @param rigidbody
 * @param forceGenerator
*/
void ForceRegistry::remove(std::shared_ptr<Rigidbody> rigidbody, std::shared_ptr<ForceGenerator> forceGenerator)
{
    RigidbodyRegistry::iterator registryIterator;
    bool registrationFound = false;

    // Pour chaque registration
    for (registryIterator = m_rigidbodyRegistry.begin(); registryIterator != m_rigidbodyRegistry.end() && !registrationFound; registryIterator++)
    {
        // Si l'association a été trouvée
        if ((*registryIterator).rigidbody == rigidbody && (*registryIterator).forceGenerator == forceGenerator)
        {
            // On la supprime
            registrationFound = true;
            m_rigidbodyRegistry.erase(registryIterator);
        }
    }
}

/**
 * @brief Retrait de toutes les associations liées au rigidbody passé en paramètre
 * @param rigidbody
*/
void ForceRegistry::remove(std::shared_ptr<Rigidbody> rigidbody)
{
    RigidbodyRegistry::iterator registryIterator = m_rigidbodyRegistry.begin();

    // Parcours de toutes les associations
    while (registryIterator != m_rigidbodyRegistry.end())
    {
        // Si l'association a été trouvée
        if ((*registryIterator).rigidbody == rigidbody)
        {
            // On la supprime
            registryIterator = m_rigidbodyRegistry.erase(registryIterator);
        }
        else
        {
            registryIterator++;
        }
    }
}


/**
 * @brief Suppression des registres (mais pas des forces/générateurs qui y étaient contenus)
*/
void ForceRegistry::clear()
{
    m_particleRegistry.clear();
    m_rigidbodyRegistry.clear();
}


/**
 * @brief Pour chaque élément des registres, met à jour l'objet courant à l'aide du générateur associé
 * @param duration 
*/
void ForceRegistry::updateForces( const float& secondsElapsedSincePreviousUpdate)
{
    for( ParticleForceRegistration& registration : m_particleRegistry )
    {
        registration.forceGenerator->updateForce( registration.particle, secondsElapsedSincePreviousUpdate);
    }

    for (RigidbodyForceRegistration& registration : m_rigidbodyRegistry)
    {
        registration.forceGenerator->updateForce(registration.rigidbody, secondsElapsedSincePreviousUpdate);
    }
}

void ForceRegistry::drawForces()
{
    for (RigidbodyForceRegistration& registration : m_rigidbodyRegistry)
    {
        registration.forceGenerator->draw(registration.rigidbody);
    }
}
