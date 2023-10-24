#include "ParticleAirFriction.h"


ParticleAirFriction::ParticleAirFriction(const Vector3& worldWind)
    : m_worldWind(worldWind)
{

}


void ParticleAirFriction::updateForce(Particle* particle, float secondsElapsedSincePreviousUpdate)
{
    bool isFrictionGlitch = false;
    const Vector3 particleVelocity = particle->getVelocity();

    // formule trainée : 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * VitesseRelativeMasseAir^2 en Newtons
    //or
    const float frontalSurface = PI * pow(particle->getRadius(), 2); // surface frontale de la particule ( = une sphère)
    const float masseVolumiqueAir = 1.2; // 1.2kg par m^3 pour air à 20°C
    const float coefSurfaceAeroDynamique = 0.45; // dans ces eaux-là pour une sphère mais à déterminer en soufflerie
    const Vector3 airRelativeVelocity = m_worldWind - particleVelocity; // Vélocité relative de la masse d'air par rapport à la particule 

    // donc
    // (On ne prend pas en compte les variations d'altitude de la particule qui impacteraient la masse volumique de l'air)
    const float normeForceTrainee = 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * pow(airRelativeVelocity.norm(), 2);

    // Le vecteur représentant la force de traînée est donc le vecteur vélocité relative de la masse d'air normalisé et multiplié par la norme de la force de trainée
    const Vector3 forceTrainee = airRelativeVelocity.normalized() * normeForceTrainee;




    // Détection de glitch
    // Loi trouvée par tâtonnements sur paint tkt : 
    // si prevision > norme(airRelativeVelocity), alors report de glitch et particleVelocity = 0 

    const float previsionVariationSurParticleVelocity = normeForceTrainee * particle->getInverseMass() * secondsElapsedSincePreviousUpdate;

    if (previsionVariationSurParticleVelocity > airRelativeVelocity.norm())
    {
        isFrictionGlitch = true;
    }



    particle->addForce(forceTrainee, isFrictionGlitch);
}
