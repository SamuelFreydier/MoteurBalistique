#include "ParticleFriction.h"


ParticleFriction::ParticleFriction( const float& frictionCoefficient, const Vector3& gravity )
    : m_frictionCoefficient( frictionCoefficient )
{
    // Que faire avec la gravit� ? => Trouver gn ("r�action � la gravit�" dans le cours) et gk 
    // Comment trouver gn et gk ? Ce sera pour une autre fois => jingle de fin de Papy Grenier
    // Au lieu de passer la gravit�, passer directement la force normale ?
}


ParticleFriction::ParticleFriction(const bool& isRealisticAirFrictionForce) // Frottements de l'air r�alistes
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

        // formule train�e : 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * VitesseRelativeMasseAir^2 en Newtons
        //or
        const float frontalSurface = PI * pow(particle->getRadius(), 2); // surface frontale de la particule ( = une sph�re)
        const float masseVolumiqueAir = 1.2; // 1.2kg par m^3 pour air � 20�C
        const float coefSurfaceAeroDynamique = 0.45; // dans ces eaux-l� pour une sph�re mais � d�terminer en soufflerie
        const Vector3 airVelocity = Engine::getInstance()->getWind(); // V�locit� de la masse d'air 
        const Vector3 airRelativeVelocity = airVelocity - particleVelocity;

        // donc
        // (air immobile donc la vitesse relative par rapport � la masse d'air est juste la vitesse de la particule)
        // (On ne prend pas en compte les variations d'altitude de la particule qui impacteraient la masse volumique de l'air)
        const float normeForceTrainee = 0.5 * frontalSurface * masseVolumiqueAir * coefSurfaceAeroDynamique * pow(airRelativeVelocity.norm(), 2);

        // Le vecteur repr�sentant la force de tra�n�e est donc le vecteur v�locit� relative de la masse d'air normalis� et multipli� par la norme de la force de train�e
        const Vector3 forceTrainee = airRelativeVelocity.normalized() * normeForceTrainee;

        const float previsionVariationSurParticleVelocity = normeForceTrainee * particle->getMassReverse() * secondsElapsedSincePreviousUpdate;
        
        
        // Bullshit ptdr :
        //  Loi trouv�e par t�tonnements sur paint tkt : 
        // Soit A = particleVelocity, B = airVelocity, C = (prevision + particleVelocity)
        // projection ortho de C sur B ne doit pas avoir une norme sup�rieure � la norme de B si cos(A,B) < 0, 
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

            // formule produit scalaire : A�B = ||A|| * ||B|| * cos(A,B)
            // Comme norme positive par d�finition, A�B est du signe de cos(A,B)
            const float cosAB_Positif = A.dotProduct(B) / (A.norm() * B.norm());

        }
        */


        // Loi trouv�e par t�tonnements sur paint tkt : 
        // si prevision > norme(airRelativeVelocity), alors report de glitch et particleVelocity = 0 

        if (previsionVariationSurParticleVelocity > airRelativeVelocity.norm())
        {
            isFrictionGlitch = true;
        }

        forceToAdd = forceTrainee;
    }

    particle->addForce(forceToAdd, isFrictionGlitch);
}
