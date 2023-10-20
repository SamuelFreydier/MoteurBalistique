#ifndef ENGINE_H
#define ENGINE_H

#include "ParticleForceGenerators/ParticleForceRegistry.h"
#include "ParticleForceGenerators/ParticleGravity.h"
#include "Fireball.h"
#include "Collision/ParticleContactResolver.h"
#include "Collision/ParticleContactGenerator.h"

class Engine
{
public:
    typedef std::vector<ParticleContactGenerator*> ContactGenerators;
    typedef std::vector<Particle*> Particles;

private:
    // Singleton
    static Engine* s_engine;

    Particles m_particles;
    
    // Registre (associations particule / générateur de force) => Gère gravité/frottements/ressorts
    ParticleForceRegistry m_particleForceRegistry;

    // Résolveur de collisions
    ParticleContactResolver m_contactResolver;

    // Vrai s'il faut calculer le nombre d'itérations à donner au résolveur de collisions à chaque frame
    bool m_calculateIterations;

    // Générateurs de collisions
    ContactGenerators m_contactGenerators;

    // Liste des collisions
    ParticleContact* m_contacts;

    // Nombre maximal de collisions autorisées
    int m_maxContacts;

    // Gravité mise à jour selon les paramètres utilisateur
    Vector3 m_gravity;

    // Frottement
    static float s_damping;

    // Variation des couleurs
    static int s_colorShift;
protected:
    Engine( const int& maxContacts, const int& iterations = 0 );
    ~Engine();

public:
    Engine( Engine& ) = delete;
    void operator=( const Engine& ) = delete;

    static Engine* getInstance( const int& maxContacts = 0, const int& iterations = 0 );

    // Tire une nouvelle particule depuis une position et avec un certain angle et une certaine force
    void shootParticle( const Vector3& initialPos, const float& initialAngle, const float& initialSpeed, const float& mass = 1.0, const float& radius = 1.0, const Vector3& color = Vector3( { 255, 0, 0 } ), bool isFireball = false );
   
    // Appelle les générateurs de collision pour signaler les collisions. Retourne le nombre de collisions générées.
    int generateContacts();
    void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

    // Mise à jour PHYSIQUE des particules
    void runPhysics( const float& deltaTime );

    // Nettoyage des particules non visibles
    void cleanup();

    // Mise à jour GRAPHIQUE des particules
    void drawParticles() const;

    // Déclenche une action si une particule est présente à l'endroit d'un clic souris. Renvoie false si rien n'a été cliqué.
    bool clickedParticle( const float& x, const float& y );

    std::vector<Particle*>& getParticles() { return m_particles; }
    void addParticle( Particle* particle ) { m_particles.push_back( particle ); }

    const Vector3& getGravity() const { return m_gravity; }
    void setGravity( const Vector3& gravity ) { m_gravity = gravity; }

    static const float& getDamping() { return s_damping; }
    static void setDamping( const float& damping ) { s_damping = damping; }

    static const int& getColorShift() { return s_colorShift; }
    static void setColorShift( const int& colorShift ) { s_colorShift = colorShift; }

    static float randshiftColorChannel( const float& colorChannel, const int& shiftAmount );
    static Vector3 randshiftColor( const Vector3& color, const int& shiftAmount );
};

#endif