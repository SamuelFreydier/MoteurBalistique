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
    
    // Registre (associations particule / g�n�rateur de force) => G�re gravit�/frottements/ressorts
    ParticleForceRegistry m_particleForceRegistry;

    // R�solveur de collisions
    ParticleContactResolver m_contactResolver;

    // Vrai s'il faut calculer le nombre d'it�rations � donner au r�solveur de collisions � chaque frame
    bool m_calculateIterations;

    // G�n�rateurs de collisions
    ContactGenerators m_contactGenerators;

    // Liste des collisions
    ParticleContact* m_contacts;

    // Nombre maximal de collisions autoris�es
    int m_maxContacts;

    // Gravit� mise � jour selon les param�tres utilisateur
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
   
    // Appelle les g�n�rateurs de collision pour signaler les collisions. Retourne le nombre de collisions g�n�r�es.
    int generateContacts();
    void addContactGenerator( ParticleContactGenerator* contactGenerator ) { m_contactGenerators.push_back( contactGenerator ); }

    // Mise � jour PHYSIQUE des particules
    void runPhysics( const float& deltaTime );

    // Nettoyage des particules non visibles
    void cleanup();

    // Mise � jour GRAPHIQUE des particules
    void drawParticles() const;

    // D�clenche une action si une particule est pr�sente � l'endroit d'un clic souris. Renvoie false si rien n'a �t� cliqu�.
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