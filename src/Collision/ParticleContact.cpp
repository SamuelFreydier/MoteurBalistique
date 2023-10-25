#include "ParticleContact.h"

void ParticleContact::resolve( const float& duration )
{
    resolveVelocity( duration );
    resolveInterpenetration( duration );
}

float ParticleContact::calculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = m_particles[ 0 ]->getVelocity();
    if( m_particles[ 1 ] )
    {
        relativeVelocity -= m_particles[ 1 ]->getVelocity();
    }

    return relativeVelocity.dotProduct( m_contactNormal );
}

void ParticleContact::resolveVelocity( const float& duration )
{
    // Trouve la vélocité dans la direction du contact
    float separatingVelocity = calculateSeparatingVelocity();
    // Est-ce que le contact se sépare déjà, ou est-ce qu'il est stationnaire ?
    if( separatingVelocity <= 0 )
    {
        // Si un de ces cas, on ne fait pas d'impulsion
        return;
    }

    // Calcul de la nouvelle vélocité de séparation
    float newSepVelocity = -separatingVelocity * m_restitution;

    // Vérifie l'augmentation de vélocité due à l'accélération seule (permet d'éviter des rebonds sur une seule frame entre deux particules collées)
    Vector3 accCausedVelocity = m_particles[ 0 ]->getAcceleration();
    if( m_particles[ 1 ] )
    {
        accCausedVelocity -= m_particles[ 1 ]->getAcceleration();
    }
    float accCausedSepVelocity = accCausedVelocity.dotProduct( m_contactNormal ) * duration;

    // Si la vélocité de rapprochement est due à une augmentation d'accélération, on la supprime de la vélocité de séparation
    if( accCausedSepVelocity >= separatingVelocity )
    {
        newSepVelocity -= accCausedSepVelocity;

        // Vérifie que l'on n'a pas enlevé plus de vélocité que nécessaire
        if( newSepVelocity > 0 )
        {
            newSepVelocity = 0;
        }
    }

    float deltaVelocity = newSepVelocity - separatingVelocity;

    // Application des changements de vélocité à chaque objet proportionnellement à leur masse
    // Plus grande masse => Moins de changement
    float totalInverseMass = m_particles[ 0 ]->getInverseMass();
    if( m_particles[ 1 ] )
    {
        totalInverseMass += m_particles[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, ça ne sert à rien de faire une impulsion, les particules resteront stoïques comme Sénèque
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Calcul de l'impulsion
    float impulse = deltaVelocity / totalInverseMass;

    // Montant d'impulsion par unité d'inverse de masse
    Vector3 impulsePerInverseMass = m_contactNormal * impulse;

    Vector3 newVelocityParticle0 = m_particles[0]->getVelocity();

    // pour appliquer la friction, on a besoin du projeté du vecteur velocity dans le plan orthogonal au vecteur contact/rebond
    // projeté du vecteur u dans le plan orthogonal au vecteur v: Proj(u) = u- ((u.n)/(n.n))*n
    Vector3 contactNormalised = m_contactNormal.normalized();
    Vector3 velocityParticle0Normalised = newVelocityParticle0;
    velocityParticle0Normalised -= contactNormalised * (velocityParticle0Normalised.dotProduct(contactNormalised) / contactNormalised.dotProduct(contactNormalised)); // m_velocityParticle0Normalized est la projection de la direction de déplacement de la balle dans le plan de la surface
    velocityParticle0Normalised = velocityParticle0Normalised.normalized();

    // Gestion des frictions
    if( m_particles[1] == nullptr ) // pour l'instant on ne considère les frictions que contre des objets inamovibles de type "sol" ou "mur", on ignore les frictions particule-particule
    {
        if (-0.2 <= separatingVelocity && separatingVelocity <= 0.2)// l'objet est au repos modulo les micro rebonds théoriquement gérés 
        {
            float forceNormale = m_particles[0]->getAcceleration().dotProduct(m_contactNormal) * duration * (-1);
            float friction;
            if (m_particles[0]->isStationary()) //la sphère est arretee, au repos
            {
                // frictions statiques
                friction = 0.6 * abs(forceNormale); //métal sur métal
                if (friction > newVelocityParticle0.dotProduct(velocityParticle0Normalised))
                {
                    friction = newVelocityParticle0.dotProduct(velocityParticle0Normalised);
                }
            }
            else
            {
                // frictions dynamiques
                friction = 0.4 * abs(forceNormale);
            }
            //newVelocityparticle0.show();
            newVelocityParticle0 -= velocityParticle0Normalised*friction;
        }
    }
    newVelocityParticle0 += impulsePerInverseMass * m_particles[0]->getInverseMass();

    // Application des impulsions => en direction du contact, proportionnelles à l'inverse de la masse, et avec ajustement post-friction
    // Première particule
    m_particles[ 0 ]->setVelocity( newVelocityParticle0 );

    // Deuxième particule
    if( m_particles[ 1 ] )
    {
        // Direction opposée donc on lui applique l'opposé de l'impulsion
        m_particles[ 1 ]->setVelocity(m_particles[1]->getVelocity() - impulsePerInverseMass * m_particles[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration( const float& duration )
{
    // S'il n'y a pas de pénétration, tout ceci est inutile
    if( m_penetration <= 0 )
    {
        return;
    }

    // Le mouvement de chaque particule est calculé proportionnellement à l'inverse de sa masse
    float totalInverseMass = m_particles[ 0 ]->getInverseMass();
    if( m_particles[ 1 ] )
    {
        totalInverseMass += m_particles[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, alors elles restent stoïques comme Epictète
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Montant de "résolution de pénétration" par unité d'inverse de masse
    Vector3 movementPerInverseMass = -m_contactNormal * ( m_penetration / totalInverseMass );

    // Application des montants de mouvement pour chaque particule pour la résolution de pénétration, proportionnellement à leur masse
    // Première particule
    m_particles[ 0 ]->setPosition( m_particles[ 0 ]->getPosition() + movementPerInverseMass * m_particles[ 0 ]->getInverseMass() );
    // Deuxième particule
    if( m_particles[ 1 ] )
    {
        // Direction opposée donc on fait attention à mettre -
        m_particles[ 1 ]->setPosition( m_particles[ 1 ]->getPosition() - movementPerInverseMass * m_particles[ 1 ]->getInverseMass() );
    }
}


