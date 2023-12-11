#include "ParticleContact.h"

/**
 * @brief R�solution d'une collision en fonction da la dur�e de la derni�re frame
 * @param duration 
*/
void Contact::resolve( const float& duration )
{
    resolveVelocity( duration );
    resolveInterpenetration( duration );
}

/**
 * @brief Calcul de la v�locit� relative dans le sens de la normale. La normale doit �tre dans le sens du contact du point de vue de la premi�re particule.
 * @return 
*/
float Contact::calculateClosingVelocity() const
{
    Vector3 relativeVelocity = m_rigidbodies[ 0 ]->getVelocity();
    if( m_rigidbodies[ 1 ] )
    {
        relativeVelocity -= m_rigidbodies[ 1 ]->getVelocity();
    }

    return relativeVelocity.dotProduct( m_contactNormal );
}


/**
 * @brief Calcul de la nouvelle v�locit� des deux acteurs de la collision (en prenant en compte les probl�mes de rebond � l'�tat stationnaire et la friction)
 * @return
*/
void Contact::resolveVelocity( const float& duration )
{
    /*
    // Trouve la v�locit� dans la direction du contact
    float closingVelocity = calculateClosingVelocity();
    // Est-ce que le contact se s�pare d�j�, ou est-ce qu'il est stationnaire ?
    if( closingVelocity <= 0 )
    {
        // Si un de ces cas, on ne fait pas d'impulsion
        return;
    }

    // Calcul de la nouvelle v�locit� de s�paration
    float newSepVelocity = -closingVelocity * m_restitution;

    // V�rifie l'augmentation de v�locit� due � l'acc�l�ration seule (permet d'�viter des rebonds sur une seule frame entre deux particules coll�es)
    Vector3 accFrame = m_rigidbodies[ 0 ]->getAcceleration();
    if( m_rigidbodies[ 1 ] )
    {
        accFrame -= m_rigidbodies[ 1 ]->getAcceleration();
    }
    float accCausedClosingVelocity = accFrame.dotProduct( m_contactNormal ) * duration;
    // Si la v�locit� de rapprochement est due � une augmentation d'acc�l�ration, on la supprime de la v�locit� de s�paration
    if( accCausedClosingVelocity >= closingVelocity )
    {
        newSepVelocity += accCausedClosingVelocity;

        // V�rifie que l'on n'a pas enlev� plus de v�locit� que n�cessaire
        if( newSepVelocity > 0 )
        {
            newSepVelocity = 0;
        }
    }

    float deltaVelocity = newSepVelocity - closingVelocity;

    // Application des changements de v�locit� � chaque objet proportionnellement � leur masse
    // Plus grande masse => Moins de changement
    float totalInverseMass = m_rigidbodies[ 0 ]->getInverseMass();
    if( m_rigidbodies[ 1 ] )
    {
        totalInverseMass += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, �a ne sert � rien de faire une impulsion, les particules resteront sto�ques comme S�n�que
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Calcul de l'impulsion
    float impulse = deltaVelocity / totalInverseMass;

    // Montant d'impulsion par unit� d'inverse de masse. L'impulsion est d�j� n�gative donc pas besoin de multiplier par -1.
    Vector3 impulsePerInverseMass = m_contactNormal * impulse;

    Vector3 newVelocityParticle0 = m_rigidbodies[ 0 ]->getVelocity();

    // pour appliquer la friction, on a besoin du projet� du vecteur velocity dans le plan orthogonal au vecteur contact/rebond
    // projet� du vecteur u dans le plan orthogonal au vecteur v: Proj(u) = u- ((u.n)/(n.n))*n
    Vector3 contactNormalised = m_contactNormal.normalized();
    Vector3 velocityParticle0Normalised = newVelocityParticle0;
    velocityParticle0Normalised -= contactNormalised * ( velocityParticle0Normalised.dotProduct( contactNormalised ) / contactNormalised.dotProduct( contactNormalised ) ); // m_velocityParticle0Normalized est la projection de la direction de d�placement de la balle dans le plan de la surface
    velocityParticle0Normalised = velocityParticle0Normalised.normalized();

    // Gestion des frictions
    if( m_rigidbodies[1] == nullptr ) // pour l'instant on ne consid�re les frictions que contre des objets inamovibles de type "sol" ou "mur", on ignore les frictions particule-particule
    {
        if (-0.2 <= closingVelocity && closingVelocity <= 0.2)// l'objet est au repos modulo les micro rebonds th�oriquement g�r�s 
        {
            float forceNormale = m_rigidbodies[0]->getAcceleration().dotProduct(m_contactNormal) * duration * (-1);
            float friction;
            if ( m_rigidbodies[0]->isStationary()) //la sph�re est arretee, au repos
            {
                // frictions statiques
                friction = 0.6 * abs(forceNormale); //m�tal sur m�tal
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

    newVelocityParticle0 += impulsePerInverseMass * m_rigidbodies[0]->getInverseMass();

    // Application des impulsions => en direction du contact, proportionnelles � l'inverse de la masse, et avec ajustement post-friction
    // Premi�re particule
    m_rigidbodies[ 0 ]->setVelocity( newVelocityParticle0 );

    // Deuxi�me particule
    if( m_rigidbodies[ 1 ] )
    {
        // Direction oppos�e donc on lui applique l'oppos� de l'impulsion
        m_rigidbodies[ 1 ]->setVelocity( m_rigidbodies[1]->getVelocity() - impulsePerInverseMass * m_rigidbodies[1]->getInverseMass());
    }
    */

    // Si pas de v�locit� de rapprochement, on arr�te
    if (getDiffVelocity().dotProduct(m_contactNormal) <= 0)
        return;


    // Calcul de l'impulsion � appliquer (k)
    float impulse = getContactImpulse(duration);


    // R�solution de la vitesse pour chaque rb
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        // D�termine s'il faut ajouter une impulsion ou son oppos�e
        int rbFactor = i == 0 ? -1 : 1;


        // R�solution de la vitesse lin�aire
        Vector3 rbLinearV = rb->getVelocity();
        rbLinearV += m_contactNormal * rbFactor * impulse * rb->getInverseMass();
        rb->setVelocity(rbLinearV);


        // R�solution de la vitesse angulaire
        Vector3 rbAngularV = rb->getAngularVelocity();
        // leftTransform permet de faire vecteur * matrice au lieu de matrice * vecteur
        rbAngularV += rb->getInverseInertiaTensorWorld().transform( (rb->getGlobalRadius(m_contactPoint) * m_contactNormal * impulse) ) * rbFactor;
        rb->setAngularVelocity(rbAngularV);
    }
}


/**
 * @brief Calcul de la nouvelle position des particules de fa�on inversement proportionnelle � leur masse, de mani�re � ce qu'il n'y ait plus d'interp�n�tration
 * @param duration 
*/
void Contact::resolveInterpenetration( const float& duration )
{
    /*
    // S'il n'y a pas de p�n�tration, tout ceci est inutile
    if( m_penetration <= 0 )
    {
        return;
    }

    // Le mouvement de chaque particule est calcul� proportionnellement � l'inverse de sa masse
    float totalInverseMass = m_rigidbodies[ 0 ]->getInverseMass();
    if( m_rigidbodies[ 1 ] )
    {
        totalInverseMass += m_rigidbodies[ 1 ]->getInverseMass();
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
    m_rigidbodies[ 0 ]->setPosition( m_rigidbodies[ 0 ]->getPosition() + movementPerInverseMass * m_rigidbodies[ 0 ]->getInverseMass() );
    // Deuxi�me particule
    if( m_rigidbodies[ 1 ] )
    {
        // Direction oppos�e donc on fait attention � mettre -
        m_rigidbodies[ 1 ]->setPosition( m_rigidbodies[ 1 ]->getPosition() - movementPerInverseMass * m_rigidbodies[ 1 ]->getInverseMass() );
    }
    */

    //Si pas d'interp�n�tration, on arr�te
    if (m_penetration <= 0)
        return;


    // Inertie totale des deux rbs
    float totalInertia = 0;

    // Inerties lin�aires des rbs
    float linearInertia[2] = { 0, 0 };

    // Inerties angulaires des rbs
    float angularInertia[2] = { 0, 0 };

    // Calcul des inerties
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        // Calcul de l'inertie lin�aire et ajout � l'inertie totale
        linearInertia[i] = getLinearInertia(rb);
        totalInertia += linearInertia[i];

        // Calcul de l'inertie angulaire et ajout � l'inertie totale
        angularInertia[i] = getAngularInertia(rb);
        totalInertia += angularInertia[i];
    }

    if (totalInertia <= 0)
        return;

    float inverseInertia = 1 / totalInertia;

    // R�solution de l'interp�n�tration
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        int rbFactor = i == 0 ? -1 : 1;

        // Calcul des mouvements lin�aires et angulaires dus � l'interp�n�tration
        float linearMove = rbFactor * m_penetration * linearInertia[i] * inverseInertia;
        float angularMove = rbFactor * m_penetration * angularInertia[i] * inverseInertia;

        // Limite de mouvement angulaire
        float angularLimitConstant = 0.2;
        float limitMove = angularLimitConstant * rb->getGlobalRadius(m_contactPoint).norm();


        // Evite des trop grosses rotations qui peuvent empirer les collisions en agravant l'interp�n�tration
        if (abs(angularMove) > limitMove)
        {
            float totalMove = linearMove + angularMove;

            if (angularMove > 0)
                angularMove = limitMove;
            else
                angularMove = -limitMove;

            linearMove = totalMove - angularMove;
        }

        // R�solution lin�aire
        rb->setPosition(rb->getPosition() + (m_contactNormal * linearMove));

        // R�solution angulaire
        Quaternion q = rb->getOrientation();
        Vector3 impulsePerMove = rb->getInverseInertiaTensorWorld() * (rb->getGlobalRadius(m_contactPoint) * m_contactNormal);
        Vector3 rotationPerMove = impulsePerMove / angularInertia[i];
        q.rotateByVector(rotationPerMove * angularMove);
        rb->setOrientation(q);
    }
}


float Contact::getContactImpulse(const float& duration) const
{
    // Calcul du num�rateur de l'impulsion
    float contactVelocity = getDiffVelocity().dotProduct(m_contactNormal);
    float restitutionImpulse = contactVelocity + getRestitution() * (contactVelocity - getAccVelocity(duration));


    //Calcul du d�nominateur de l'impulsion
    Vector3 restitutionImpulseDividerVector;
    float restitutionImpulseDivider;

    //Partie lin�aire
    float totalInverseMass = m_rigidbodies[0]->getInverseMass();
    if (m_rigidbodies[1])
        totalInverseMass += m_rigidbodies[1]->getInverseMass();
    restitutionImpulseDividerVector += m_contactNormal * totalInverseMass;

    //Partie angulaire
    restitutionImpulseDividerVector += getAngularImpulseDivider(m_rigidbodies[0]);
    if (m_rigidbodies[1])
        restitutionImpulseDividerVector += getAngularImpulseDivider(m_rigidbodies[1]);

    restitutionImpulseDivider = restitutionImpulseDividerVector.dotProduct(m_contactNormal);


    return restitutionImpulse / restitutionImpulseDivider;
}



float Contact::getRestitution() const
{
    return m_restitution;
}


Vector3 Contact::getDiffVelocity() const
{
    Vector3 diffVelocity = m_rigidbodies[0]->getTotalVelocity(m_contactPoint);
    if (m_rigidbodies[1])
        diffVelocity -= m_rigidbodies[1]->getTotalVelocity(m_contactPoint);

    return diffVelocity;
}


float Contact::getAccVelocity(const float& duration) const
{
    Vector3 acc = m_rigidbodies[0]->getAcceleration();
    if (m_rigidbodies[1])
        acc -= m_rigidbodies[1]->getAcceleration();

    return acc.dotProduct(m_contactNormal) * duration;
}

Vector3 Contact::getAngularImpulseDivider(std::shared_ptr<Rigidbody> rb) const
{
    Vector3 globalRadiusRb = rb->getGlobalRadius(m_contactPoint);

    // result = ((r * n) * J^-1) * r
    return (rb->getInverseInertiaTensorWorld().transform(globalRadiusRb * m_contactNormal)) * globalRadiusRb;
}


float Contact::getLinearInertia(std::shared_ptr<Rigidbody> rb) const
{
    return rb->getInverseMass();
}

float Contact::getAngularInertia(std::shared_ptr<Rigidbody> rb) const
{
    Vector3 globalRadius = rb->getGlobalRadius(m_contactPoint);

    Vector3 angularInertiaVector = globalRadius * m_contactNormal;
    angularInertiaVector = (rb->getInverseInertiaTensorWorld() * angularInertiaVector) * globalRadius;
    
    return angularInertiaVector.dotProduct(m_contactNormal);
}