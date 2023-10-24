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
    // Trouve la v�locit� dans la direction du contact
    float separatingVelocity = calculateSeparatingVelocity();

    // Est-ce que le contact se s�pare d�j�, ou est-ce qu'il est stationnaire ?
    if( separatingVelocity <= 0 )
    {
        // Si un de ces cas, on ne fait pas d'impulsion
        return;
    }

    // Calcul de la nouvelle v�locit� de s�paration
    float newSepVelocity = -separatingVelocity * m_restitution;

    // V�rifie l'augmentation de v�locit� due � l'acc�l�ration seule (permet d'�viter des rebonds sur une seule frame entre deux particules coll�es)
    Vector3 accCausedVelocity = m_particles[ 0 ]->getAcceleration();
    if( m_particles[ 1 ] )
    {
        accCausedVelocity -= m_particles[ 1 ]->getAcceleration();
    }
    float accCausedSepVelocity = accCausedVelocity.dotProduct( m_contactNormal ) * duration;
    //std::cout << "AccCausedSepVelocity: " << accCausedSepVelocity << '\n';
    // Si la v�locit� de rapprochement est due � une augmentation d'acc�l�ration, on la supprime de la v�locit� de s�paration
    if( accCausedSepVelocity >= separatingVelocity )
    {
        newSepVelocity -= accCausedSepVelocity;

        // V�rifie que l'on n'a pas enlev� plus de v�locit� que n�cessaire
        if( newSepVelocity < 0 )
        {
            newSepVelocity = 0;
        }
    }

    float deltaVelocity = newSepVelocity - separatingVelocity;

    //std::cout << "New Sep Velocity: " << newSepVelocity << '\n';
    //std::cout << "Separating Velocity : " << separatingVelocity << '\n';
    //std::cout << "Delta Velocity: " << deltaVelocity << '\n';

    // Application des changements de v�locit� � chaque objet proportionnellement � leur masse
    // Plus grande masse => Moins de changement
    float totalInverseMass = m_particles[ 0 ]->getInverseMass();
    if( m_particles[ 1 ] )
    {
        totalInverseMass += m_particles[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, �a ne sert � rien de faire une impulsion, les particules resteront sto�ques comme S�n�que
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Calcul de l'impulsion
    float impulse = deltaVelocity / totalInverseMass;

    // Montant d'impulsion par unit� d'inverse de masse
    Vector3 impulsePerInverseMass = m_contactNormal * impulse;
    //impulsePerInverseMass.show();

    // Application des impulsions => en direction du contact et proportionnelles � l'inverse de la masse
    // Premi�re particule
    //std::cout << "Before Velocity : ";
    //m_particles[ 0 ]->getVelocity().show();
    //Vector3( impulsePerInverseMass * m_particles[ 0 ]->getInverseMass() ).show();
    m_particles[ 0 ]->setVelocity( m_particles[ 0 ]->getVelocity() + impulsePerInverseMass * m_particles[ 0 ]->getInverseMass() );
    //std::cout << "After Velocity : ";
    //m_particles[ 0 ]->getVelocity().show();
    // Deuxi�me particule
    if( m_particles[ 1 ] )
    {
        // Direction oppos�e donc on lui applique l'oppos� de l'impulsion
        m_particles[ 1 ]->setVelocity( m_particles[ 1 ]->getVelocity() - impulsePerInverseMass * m_particles[ 1 ]->getInverseMass() );
    }


}

void ParticleContact::resolveInterpenetration( const float& duration )
{
    // S'il n'y a pas de p�n�tration, tout ceci est inutile
    if( m_penetration <= 0 )
    {
        return;
    }

    // Le mouvement de chaque particule est calcul� proportionnellement � l'inverse de sa masse
    float totalInverseMass = m_particles[ 0 ]->getInverseMass();
    if( m_particles[ 1 ] )
    {
        totalInverseMass += m_particles[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, alors elles restent sto�ques comme Epict�te
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Montant de "r�solution de p�n�tration" par unit� d'inverse de masse
    Vector3 movementPerInverseMass = -m_contactNormal * ( m_penetration / totalInverseMass );

    // Application des montants de mouvement pour chaque particule pour la r�solution de p�n�tration, proportionnellement � leur masse
    // Premi�re particule
    //std::cout << "Floor height: " << ofGetWindowHeight() << '\n';
    //std::cout << "Particle radius: " << m_particles[ 0 ]->getRadius() << '\n';
    //std::cout << "Position before: ";
    //m_particles[ 0 ]->getPosition().show();
    //std::cout << "Movement Per Inverse Mass";
    //movementPerInverseMass.show();
    m_particles[ 0 ]->setPosition( m_particles[ 0 ]->getPosition() + movementPerInverseMass * m_particles[ 0 ]->getInverseMass() );
    //std::cout << "Position after: ";
    //m_particles[ 0 ]->getPosition().show();
    // Deuxi�me particule
    if( m_particles[ 1 ] )
    {
        // Direction oppos�e donc on fait attention � mettre -
        m_particles[ 1 ]->setPosition( m_particles[ 1 ]->getPosition() - movementPerInverseMass * m_particles[ 1 ]->getInverseMass() );
    }
}
