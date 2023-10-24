#include "ParticleForceRegistry.h"

/**
 * @brief Ajout d'une association particule/force dans le registre
 * @param particle 
 * @param forceGenerator 
*/
void ParticleForceRegistry::add( Particle* particle, ParticleForceGenerator* forceGenerator )
{
    m_registry.push_back( ParticleForceRegistration( { particle, forceGenerator } ) );
}

/**
 * @brief Retrait d'une association particule/force du registre
 * @param particle 
 * @param forceGenerator 
*/
void ParticleForceRegistry::remove( Particle* particle, ParticleForceGenerator* forceGenerator )
{
    Registry::iterator registryIterator;
    bool registrationFound = false;

    // Pour chaque registration
    for( registryIterator = m_registry.begin(); registryIterator != m_registry.end() && !registrationFound; registryIterator++ )
    {
        // Si l'association a �t� trouv�e
        if( ( *registryIterator ).particle == particle && ( *registryIterator ).forceGenerator == forceGenerator )
        {
            // On la supprime
            registrationFound = true;
            m_registry.erase( registryIterator );
        }
    }
}

/**
 * @brief Retrait de toutes les associations li�es � la particule pass�e en param�tre
 * @param particle 
*/
void ParticleForceRegistry::remove( Particle* particle )
{
    Registry::iterator registryIterator = m_registry.begin();

    // Parcours de toutes les associations
    while( registryIterator != m_registry.end() )
    {
        // Si l'association a �t� trouv�e
        if( ( *registryIterator ).particle == particle )
        {
            // On la supprime
            registryIterator = m_registry.erase( registryIterator );
        }
        else
        {
            registryIterator++;
        }
    }
}


/**
 * @brief Suppression du registre (mais pas des forces/g�n�rateurs qui y �taient contenus)
*/
void ParticleForceRegistry::clear()
{
    m_registry.clear();
}


/**
 * @brief Pour chaque �l�ment du registre, met � jour la particule courante � l'aide du g�n�rateur associ�
 * @param duration 
*/
void ParticleForceRegistry::updateForces( const float& secondsElapsedSincePreviousUpdate)
{
    for( ParticleForceRegistration& registration : m_registry )
    {
        registration.forceGenerator->updateForce( registration.particle, secondsElapsedSincePreviousUpdate);
    }
}
