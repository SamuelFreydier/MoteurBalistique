#include "ParticleFriction.h"


ParticleFriction::ParticleFriction( const float& frictionCoefficient, const Vector3& gravity )
    : m_frictionCoefficient( frictionCoefficient )
{
    // Que faire avec la gravité ? => Trouver gn ("réaction à la gravité" dans le cours) et gk 
    // Comment trouver gn et gk ? Ce sera pour une autre fois => jingle de fin de Papy Grenier
    // Au lieu de passer la gravité, passer directement la force normale ?
}


ParticleFriction::ParticleFriction(const bool& isRealisticAirFrictionForce) // Frottements de l'air réalistes
    : m_isRealisticAirFrictionForce(isRealisticAirFrictionForce)
{

}


void ParticleFriction::updateForce( const std::shared_ptr<Particle>& particle, const float& secondsElapsedSincePreviousUpdate)
{
    Vector3 forceToAdd;
    bool isFrictionGlitch = false;

    if (m_isRealisticAirFrictionForce)
    {
        const Vector3 particleVelocity = particle->getVelocity();

        // formule trainée : 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * VitesseRelativeMasseAir^2 en Newtons
        //or
        const float frontalSurface = PI * pow(particle->getRadius(), 2); // surface frontale de la particule ( = une sphère)
        const float masseVolumiqueAir = 1.2; // 1.2kg par m^3 pour air à 20°C
        const float coefSurfaceAeroDynamique = 0.45; // dans ces eaux-là pour une sphère mais à déterminer en soufflerie
        const Vector3 airVelocity = Engine::getInstance()->getWind(); // Vélocité de la masse d'air 
        const Vector3 airRelativeVelocity = airVelocity - particleVelocity;

        // donc
        // (air immobile donc la vitesse relative par rapport à la masse d'air est juste la vitesse de la particule)
        // (On ne prend pas en compte les variations d'altitude de la particule qui impacteraient la masse volumique de l'air)
        const float normeForceTrainee = 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * pow(airRelativeVelocity.norm(), 2);

        // Le vecteur représentant la force de traînée est donc le vecteur vélocité relative de la masse d'air normalisé et multiplié par la norme de la force de trainée
        const Vector3 forceTrainee = airRelativeVelocity.normalized() * normeForceTrainee;

        const float previsionVariationSurParticleVelocity = normeForceTrainee * particle->getMassReverse() * secondsElapsedSincePreviousUpdate;
        
        
        // Bullshit ptdr :
        //  Loi trouvée par tâtonnements sur paint tkt : 
        // Soit A = particleVelocity, B = airVelocity, C = (prevision + particleVelocity)
        // projection ortho de C sur B ne doit pas avoir une norme supérieure à la norme de B si cos(A,B) < 0, 
        // l'inverse si cos(A,B) > 0
        // si B vecteur nul, alors il faut que norme(prevision) <= norme(particleVelocity)
        // si un des trois cas survient, alors particleVelocity = 0 et report de glitch

        /* re-Bullshit ptdr
        if (airVelocity == Vector3())
        {
            if (previsionVariationSurParticleVelocity.norm() > particleVelocity.norm())
            {
                isFrictionGlitch = true;
            }
        }
        else
        {
            const Vector3 A = particleVelocity;
            const Vector3 B = airVelocity;
            const Vector3 C = previsionVariationSurParticleVelocity + particleVelocity;

            // formule produit scalaire : A·B = ||A|| * ||B|| * cos(A,B)
            // Comme norme positive par définition, A·B est du signe de cos(A,B)
            const float cosAB_Positif = A.dotProduct(B) / (A.norm() * B.norm());

        }
        */


        // Loi trouvée par tâtonnements sur paint tkt : 
        // si prevision > norme(airRelativeVelocity), alors report de glitch et particleVelocity = 0 

        if (previsionVariationSurParticleVelocity > airRelativeVelocity.norm())
        {
            isFrictionGlitch = true;
        }

        forceToAdd = forceTrainee;
    }

    particle->addForce(forceToAdd, isFrictionGlitch);
}
