#include "ParticleContact.h"

/**
 * @brief Résolution d'une collision en fonction da la durée de la dernière frame
 * @param duration 
*/
void Contact::resolve( const float& duration )
{
    resolveVelocity( duration );
    resolveInterpenetration( duration );
}

/**
 * @brief Calcul de la vélocité relative dans le sens de la normale. La normale doit être dans le sens du contact du point de vue de la première particule.
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
 * @brief Calcul de la nouvelle vélocité des deux acteurs de la collision (en prenant en compte les problèmes de rebond à l'état stationnaire et la friction)
 * @return
*/
void Contact::resolveVelocity( const float& duration )
{
    /*
    // Trouve la vélocité dans la direction du contact
    float closingVelocity = calculateClosingVelocity();
    // Est-ce que le contact se sépare déjà, ou est-ce qu'il est stationnaire ?
    if( closingVelocity <= 0 )
    {
        // Si un de ces cas, on ne fait pas d'impulsion
        return;
    }

    // Calcul de la nouvelle vélocité de séparation
    float newSepVelocity = -closingVelocity * m_restitution;

    // Vérifie l'augmentation de vélocité due à l'accélération seule (permet d'éviter des rebonds sur une seule frame entre deux particules collées)
    Vector3 accFrame = m_rigidbodies[ 0 ]->getAcceleration();
    if( m_rigidbodies[ 1 ] )
    {
        accFrame -= m_rigidbodies[ 1 ]->getAcceleration();
    }
    float accCausedClosingVelocity = accFrame.dotProduct( m_contactNormal ) * duration;
    // Si la vélocité de rapprochement est due à une augmentation d'accélération, on la supprime de la vélocité de séparation
    if( accCausedClosingVelocity >= closingVelocity )
    {
        newSepVelocity += accCausedClosingVelocity;

        // Vérifie que l'on n'a pas enlevé plus de vélocité que nécessaire
        if( newSepVelocity > 0 )
        {
            newSepVelocity = 0;
        }
    }

    float deltaVelocity = newSepVelocity - closingVelocity;

    // Application des changements de vélocité à chaque objet proportionnellement à leur masse
    // Plus grande masse => Moins de changement
    float totalInverseMass = m_rigidbodies[ 0 ]->getInverseMass();
    if( m_rigidbodies[ 1 ] )
    {
        totalInverseMass += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Si toutes les particules ont une masse infinie, ça ne sert à rien de faire une impulsion, les particules resteront stoïques comme Sénèque
    if( totalInverseMass <= 0 )
    {
        return;
    }

    // Calcul de l'impulsion
    float impulse = deltaVelocity / totalInverseMass;

    // Montant d'impulsion par unité d'inverse de masse. L'impulsion est déjà négative donc pas besoin de multiplier par -1.
    Vector3 impulsePerInverseMass = m_contactNormal * impulse;

    Vector3 newVelocityParticle0 = m_rigidbodies[ 0 ]->getVelocity();

    // pour appliquer la friction, on a besoin du projeté du vecteur velocity dans le plan orthogonal au vecteur contact/rebond
    // projeté du vecteur u dans le plan orthogonal au vecteur v: Proj(u) = u- ((u.n)/(n.n))*n
    Vector3 contactNormalised = m_contactNormal.normalized();
    Vector3 velocityParticle0Normalised = newVelocityParticle0;
    velocityParticle0Normalised -= contactNormalised * ( velocityParticle0Normalised.dotProduct( contactNormalised ) / contactNormalised.dotProduct( contactNormalised ) ); // m_velocityParticle0Normalized est la projection de la direction de déplacement de la balle dans le plan de la surface
    velocityParticle0Normalised = velocityParticle0Normalised.normalized();

    // Gestion des frictions
    if( m_rigidbodies[1] == nullptr ) // pour l'instant on ne considère les frictions que contre des objets inamovibles de type "sol" ou "mur", on ignore les frictions particule-particule
    {
        if (-0.2 <= closingVelocity && closingVelocity <= 0.2)// l'objet est au repos modulo les micro rebonds théoriquement gérés 
        {
            float forceNormale = m_rigidbodies[0]->getAcceleration().dotProduct(m_contactNormal) * duration * (-1);
            float friction;
            if ( m_rigidbodies[0]->isStationary()) //la sphère est arretee, au repos
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

    newVelocityParticle0 += impulsePerInverseMass * m_rigidbodies[0]->getInverseMass();

    // Application des impulsions => en direction du contact, proportionnelles à l'inverse de la masse, et avec ajustement post-friction
    // Première particule
    m_rigidbodies[ 0 ]->setVelocity( newVelocityParticle0 );

    // Deuxième particule
    if( m_rigidbodies[ 1 ] )
    {
        // Direction opposée donc on lui applique l'opposé de l'impulsion
        m_rigidbodies[ 1 ]->setVelocity( m_rigidbodies[1]->getVelocity() - impulsePerInverseMass * m_rigidbodies[1]->getInverseMass());
    }
    */

    // Si pas de vélocité de rapprochement, on arrête
    if (getDiffVelocity().dotProduct(m_contactNormal) <= 0)
        return;


    // Calcul de l'impulsion à appliquer (k)
    float impulse = getContactImpulse(duration);


    // Résolution de la vitesse pour chaque rb
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        // Détermine s'il faut ajouter une impulsion ou son opposée
        int rbFactor = i == 0 ? -1 : 1;


        // Résolution de la vitesse linéaire
        Vector3 rbLinearV = rb->getVelocity();
        rbLinearV += m_contactNormal * rbFactor * impulse * rb->getInverseMass();
        rb->setVelocity(rbLinearV);


        // Résolution de la vitesse angulaire
        Vector3 rbAngularV = rb->getAngularVelocity();
        // leftTransform permet de faire vecteur * matrice au lieu de matrice * vecteur
        rbAngularV += rb->getInverseInertiaTensorWorld().transform( (rb->getGlobalRadius(m_contactPoint) * m_contactNormal * impulse) ) * rbFactor;
        rb->setAngularVelocity(rbAngularV);
    }
}


/**
 * @brief Calcul de la nouvelle position des particules de façon inversement proportionnelle à leur masse, de manière à ce qu'il n'y ait plus d'interpénétration
 * @param duration 
*/
void Contact::resolveInterpenetration( const float& duration )
{
    /*
    // S'il n'y a pas de pénétration, tout ceci est inutile
    if( m_penetration <= 0 )
    {
        return;
    }

    // Le mouvement de chaque particule est calculé proportionnellement à l'inverse de sa masse
    float totalInverseMass = m_rigidbodies[ 0 ]->getInverseMass();
    if( m_rigidbodies[ 1 ] )
    {
        totalInverseMass += m_rigidbodies[ 1 ]->getInverseMass();
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
    m_rigidbodies[ 0 ]->setPosition( m_rigidbodies[ 0 ]->getPosition() + movementPerInverseMass * m_rigidbodies[ 0 ]->getInverseMass() );
    // Deuxième particule
    if( m_rigidbodies[ 1 ] )
    {
        // Direction opposée donc on fait attention à mettre -
        m_rigidbodies[ 1 ]->setPosition( m_rigidbodies[ 1 ]->getPosition() - movementPerInverseMass * m_rigidbodies[ 1 ]->getInverseMass() );
    }
    */

    //Si pas d'interpénétration, on arrête
    if (m_penetration <= 0)
        return;


    // Inertie totale des deux rbs
    float totalInertia = 0;

    // Inerties linéaires des rbs
    float linearInertia[2] = { 0, 0 };

    // Inerties angulaires des rbs
    float angularInertia[2] = { 0, 0 };

    // Calcul des inerties
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        // Calcul de l'inertie linéaire et ajout à l'inertie totale
        linearInertia[i] = getLinearInertia(rb);
        totalInertia += linearInertia[i];

        // Calcul de l'inertie angulaire et ajout à l'inertie totale
        angularInertia[i] = getAngularInertia(rb);
        totalInertia += angularInertia[i];
    }

    if (totalInertia <= 0)
        return;

    float inverseInertia = 1 / totalInertia;

    // Résolution de l'interpénétration
    for (int i = 0; i <= 1; i++)
    {
        auto& rb = m_rigidbodies[i];

        if (!rb)
            continue;

        int rbFactor = i == 0 ? -1 : 1;

        // Calcul des mouvements linéaires et angulaires dus à l'interpénétration
        float linearMove = rbFactor * m_penetration * linearInertia[i] * inverseInertia;
        float angularMove = rbFactor * m_penetration * angularInertia[i] * inverseInertia;

        // Limite de mouvement angulaire
        float angularLimitConstant = 0.2;
        float limitMove = angularLimitConstant * rb->getGlobalRadius(m_contactPoint).norm();


        // Evite des trop grosses rotations qui peuvent empirer les collisions en agravant l'interpénétration
        if (abs(angularMove) > limitMove)
        {
            float totalMove = linearMove + angularMove;

            if (angularMove > 0)
                angularMove = limitMove;
            else
                angularMove = -limitMove;

            linearMove = totalMove - angularMove;
        }

        // Résolution linéaire
        rb->setPosition(rb->getPosition() + (m_contactNormal * linearMove));

        // Résolution angulaire
        Quaternion q = rb->getOrientation();
        Vector3 impulsePerMove = rb->getInverseInertiaTensorWorld() * (rb->getGlobalRadius(m_contactPoint) * m_contactNormal);
        Vector3 rotationPerMove = impulsePerMove / angularInertia[i];
        q.rotateByVector(rotationPerMove * angularMove);
        rb->setOrientation(q);
    }
}


float Contact::getContactImpulse(const float& duration) const
{
    // Calcul du numérateur de l'impulsion
    float contactVelocity = getDiffVelocity().dotProduct(m_contactNormal);
    float restitutionImpulse = contactVelocity + getRestitution() * (contactVelocity - getAccVelocity(duration));


    //Calcul du dénominateur de l'impulsion
    Vector3 restitutionImpulseDividerVector;
    float restitutionImpulseDivider;

    //Partie linéaire
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