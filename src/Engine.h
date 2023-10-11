#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleForceGenerators/ParticleForceRegistry.h"
#include "ParticleForceGenerators/ParticleGravity.h"
#include "Fireball.h"

using ParticlePtr = std::shared_ptr<Particle>;

class Engine
{
private:
    // Singleton
    static Engine* s_engine;

    std::list<ParticlePtr> m_particles;
    
    // Registre (associations particule / g�n�rateur de force) => G�re gravit�/frottements/ressorts
    ParticleForceRegistry m_particleForceRegistry;

    // Gravit� mise � jour selon les param�tres utilisateur
    Vector m_gravity;

    // Frottement
    static float s_damping;

    // Variation des couleurs
    static int s_colorShift;
protected:
    Engine();
public:
    Engine( Engine& ) = delete;
    void operator=( const Engine& ) = delete;

    static Engine* getInstance();

    const std::list<ParticlePtr>& getParticles() const { return m_particles; }
    void addParticle( ParticlePtr particle ) { m_particles.push_back( particle ); }
    void deleteParticle( ParticlePtr particle ) { m_particles.remove( particle ); }

    const Vector& getGravity() const { return m_gravity; }
    void setGravity( const Vector& gravity ) { m_gravity = gravity; }

    static const float& getDamping() { return s_damping; }
    static void setDamping( const float& damping ) { s_damping = damping; }

    static const int& getColorShift() { return s_colorShift; }
    static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

    static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
    static Vector randshiftColor( const Vector& color, const int& shiftAmount );

    // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
    void shootParticle( const Vector& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass = 1.0, const float& radius = 1.0, const Vector& color = Vector( { 255, 0, 0 } ), bool isFireball = false );
   
    // Mise � jour PHYSIQUE des particules
    void update( const float& deltaTime );

    // Nettoyage des particules non visibles
    void cleanup();

    // Mise � jour GRAPHIQUE des particules
    void drawParticles() const;

    // D�clenche une action si une particule est pr�sente � l'endroit d'un clic souris. Renvoie false si rien n'a �t� cliqu�.
    bool clickedParticle( const float& x, const float& y );
};

#endif