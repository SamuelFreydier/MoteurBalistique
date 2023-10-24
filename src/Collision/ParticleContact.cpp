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
    //std::cout << "AccCausedSepVelocity: " << accCausedSepVelocity << '\n';
    // Si la vélocité de rapprochement est due à une augmentation d'accélération, on la supprime de la vélocité de séparation
    if( accCausedSepVelocity >= separatingVelocity )
    {
        newSepVelocity -= accCausedSepVelocity;

        // Vérifie que l'on n'a pas enlevé plus de vélocité que nécessaire
        if( newSepVelocity < 0 )
        {
            newSepVelocity = 0;
        }
    }

    float deltaVelocity = newSepVelocity - separatingVelocity;

    //std::cout << "New Sep Velocity: " << newSepVelocity << '\n';
    //std::cout << "Separating Velocity : " << separatingVelocity << '\n';
    //std::cout << "Delta Velocity: " << deltaVelocity << '\n';

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
    //impulsePerInverseMass.show();

    // Application des impulsions => en direction du contact et proportionnelles à l'inverse de la masse
    // Première particule
    //std::cout << "Before Velocity : ";
    //m_particles[ 0 ]->getVelocity().show();
    //Vector3( impulsePerInverseMass * m_particles[ 0 ]->getInverseMass() ).show();
    m_particles[ 0 ]->setVelocity( m_particles[ 0 ]->getVelocity() + impulsePerInverseMass * m_particles[ 0 ]->getInverseMass() );
    //std::cout << "After Velocity : ";
    //m_particles[ 0 ]->getVelocity().show();
    // Deuxième particule
    if( m_particles[ 1 ] )
    {
        // Direction opposée donc on lui applique l'opposé de l'impulsion
        m_particles[ 1 ]->setVelocity( m_particles[ 1 ]->getVelocity() - impulsePerInverseMass * m_particles[ 1 ]->getInverseMass() );
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
    //std::cout << "Floor height: " << ofGetWindowHeight() << '\n';
    //std::cout << "Particle radius: " << m_particles[ 0 ]->getRadius() << '\n';
    //std::cout << "Position before: ";
    //m_particles[ 0 ]->getPosition().show();
    //std::cout << "Movement Per Inverse Mass";
    //movementPerInverseMass.show();
    m_particles[ 0 ]->setPosition( m_particles[ 0 ]->getPosition() + movementPerInverseMass * m_particles[ 0 ]->getInverseMass() );
    //std::cout << "Position after: ";
    //m_particles[ 0 ]->getPosition().show();
    // Deuxième particule
    if( m_particles[ 1 ] )
    {
        // Direction opposée donc on fait attention à mettre -
        m_particles[ 1 ]->setPosition( m_particles[ 1 ]->getPosition() - movementPerInverseMass * m_particles[ 1 ]->getInverseMass() );
    }
}
