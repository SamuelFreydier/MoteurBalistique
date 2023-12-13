#include "ParticleContact.h"

Vector3 Contact::calculateFrictionlessImpulse( Matrix3x3* inverseInertiaTensor )
{
    Vector3 impulseContact;

    // Build a vector that shows the change in velocity in
    // world space for a unit impulse in the direction of the contact
    // normal.
    Vector3 deltaVelWorld = relativeContactPosition[ 0 ] * m_contactNormal;
    deltaVelWorld = inverseInertiaTensor[ 0 ].transform( deltaVelWorld );
    deltaVelWorld = deltaVelWorld * relativeContactPosition[ 0 ];

    // Work out the change in velocity in contact coordiantes.
    float deltaVelocity = deltaVelWorld.dotProduct( m_contactNormal );

    // Add the linear component of velocity change
    deltaVelocity += m_rigidbodies[ 0 ]->getInverseMass();

    // Check if we need to the second body's data
    if( m_rigidbodies[ 1 ] )
    {
        // Go through the same transformation sequence again
        Vector3 deltaVelWorld = relativeContactPosition[ 1 ] * m_contactNormal;
        deltaVelWorld = inverseInertiaTensor[ 1 ].transform( deltaVelWorld );
        deltaVelWorld = deltaVelWorld * relativeContactPosition[ 1 ];

        // Add the change in velocity due to rotation
        deltaVelocity += deltaVelWorld.dotProduct( m_contactNormal );

        // Add the change in velocity due to linear motion
        deltaVelocity += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Calculate the required size of the impulse
    impulseContact.x = desiredDeltaVelocity / deltaVelocity;
    impulseContact.y = 0;
    impulseContact.z = 0;
    return impulseContact;
}

Vector3 Contact::calculateFrictionImpulse( Matrix3x3* inverseInertiaTensor )
{
    Vector3 impulseContact;
    float inverseMass = m_rigidbodies[ 0 ]->getInverseMass();

    // The equivalent of a cross product in matrices is multiplication
    // by a skew symmetric matrix - we build the matrix for converting
    // between linear and angular quantities.
    Matrix3x3 impulseToTorque;
    impulseToTorque.setSkewSymmetric( relativeContactPosition[ 0 ] );

    // Build the matrix to convert contact impulse to change in velocity
    // in world coordinates.
    Matrix3x3 deltaVelWorld = impulseToTorque;
    deltaVelWorld *= inverseInertiaTensor[ 0 ];
    deltaVelWorld *= impulseToTorque;
    deltaVelWorld *= -1;

    // Check if we need to add body two's data
    if( m_rigidbodies[ 1 ] )
    {
        // Set the cross product matrix
        impulseToTorque.setSkewSymmetric( relativeContactPosition[ 1 ] );

        // Calculate the velocity change matrix
        Matrix3x3 deltaVelWorld2 = impulseToTorque;
        deltaVelWorld2 *= inverseInertiaTensor[ 1 ];
        deltaVelWorld2 *= impulseToTorque;
        deltaVelWorld2 *= -1;

        // Add to the total delta velocity.
        deltaVelWorld += deltaVelWorld2;

        // Add to the inverse mass
        inverseMass += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Do a change of basis to convert into contact coordinates.
    Matrix3x3 deltaVelocity = contactToWorld.transposed();
    deltaVelocity *= deltaVelWorld;
    deltaVelocity *= contactToWorld;

    // Add in the linear velocity change
    deltaVelocity[ 0 ] += inverseMass;
    deltaVelocity[ 4 ] += inverseMass;
    deltaVelocity[ 8 ] += inverseMass;

    // Invert to get the impulse needed per unit velocity
    Matrix3x3 impulseMatrix = deltaVelocity.inverted();

    // Find the target velocities to kill
    Vector3 velKill( desiredDeltaVelocity,
                     -contactVelocity.y,
                     -contactVelocity.z );

    // Find the impulse to kill target velocities
    impulseContact = impulseMatrix.transform( velKill );

    // Check for exceeding friction
    float planarImpulse = sqrtf(
        impulseContact.y * impulseContact.y +
        impulseContact.z * impulseContact.z
    );
    if( planarImpulse > impulseContact.x * m_friction )
    {
        // We need to use dynamic friction
        impulseContact.y /= planarImpulse;
        impulseContact.z /= planarImpulse;

        impulseContact.x = deltaVelocity[ 0 ] +
            deltaVelocity[ 1 ] * m_friction * impulseContact.y +
            deltaVelocity[ 2 ] * m_friction * impulseContact.z;
        impulseContact.x = desiredDeltaVelocity / impulseContact.x;
        impulseContact.y *= m_friction * impulseContact.x;
        impulseContact.z *= m_friction * impulseContact.x;
    }
    return impulseContact;
}

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

void Contact::setBodyData( std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two, float friction, float restitution )
{
    m_rigidbodies[ 0 ] = one;
    m_rigidbodies[ 1 ] = two;
    m_friction = friction;
    m_restitution = restitution;
}

void Contact::calculateInternals( float duration )
{
    if( !m_rigidbodies[ 0 ] ) swapBodies();

    // Calculate an set of axis at the contact point.
    calculateContactBasis();

    // Store the relative position of the contact relative to each body
    relativeContactPosition[ 0 ] = m_contactPoint - m_rigidbodies[ 0 ]->getPosition();
    if( m_rigidbodies[ 1 ] ) {
        relativeContactPosition[ 1 ] = m_contactPoint - m_rigidbodies[ 1 ]->getPosition();
    }

    // Find the relative velocity of the bodies at the contact point.
    contactVelocity = calculateLocalVelocity( 0, duration );
    if( m_rigidbodies[ 1 ] ) {
        contactVelocity -= calculateLocalVelocity( 1, duration );
    }

    // Calculate the desired change in velocity for resolution
    calculateDesiredDeltaVelocity( duration );
}

void Contact::swapBodies()
{
    m_contactNormal *= -1;

    std::shared_ptr<Rigidbody> temp = m_rigidbodies[ 0 ];
    m_rigidbodies[ 0 ] = m_rigidbodies[ 1 ];
    m_rigidbodies[ 1 ] = temp;
}

void Contact::calculateDesiredDeltaVelocity( float duration )
{
    const static float velocityLimit = ( float ) 0.25f;

    // Calculate the acceleration induced velocity accumulated this frame
    float velocityFromAcc = 0;

    velocityFromAcc +=
        (m_rigidbodies[ 0 ]->getAcceleration() * duration).dotProduct(m_contactNormal);

    if( m_rigidbodies[ 1 ] )
    {
        velocityFromAcc -=
            (m_rigidbodies[ 1 ]->getAcceleration() * duration).dotProduct(m_contactNormal);
    }

    // If the velocity is very slow, limit the restitution
    float thisRestitution = m_restitution;
    if( abs( contactVelocity.x ) < velocityLimit )
    {
        thisRestitution = ( float ) 0.0f;
    }

    // Combine the bounce velocity with the removed
    // acceleration velocity.
    desiredDeltaVelocity =
        -contactVelocity.x
        - thisRestitution * ( contactVelocity.x - velocityFromAcc );
}

Vector3 Contact::calculateLocalVelocity( unsigned bodyIndex, float duration )
{
    std::shared_ptr<Rigidbody> thisBody = m_rigidbodies[ bodyIndex ];

    // Work out the velocity of the contact point.
    Vector3 velocity =
        thisBody->getAngularVelocity() * relativeContactPosition[bodyIndex];
    velocity += thisBody->getVelocity();

    // Turn the velocity into contact-coordinates.
    Vector3 contactVelocity = contactToWorld.transformTranspose( velocity );

    // Calculate the ammount of velocity that is due to forces without
    // reactions.
    Vector3 accVelocity = thisBody->getAcceleration() * duration;

    // Calculate the velocity in contact-coordinates.
    accVelocity = contactToWorld.transformTranspose( accVelocity );

    // We ignore any component of acceleration in the contact normal
    // direction, we are only interested in planar acceleration
    accVelocity.x = 0;

    // Add the planar velocities - if there's enough friction they will
    // be removed during velocity resolution
    contactVelocity += accVelocity;

    // And return it
    return contactVelocity;
}

void Contact::calculateContactBasis()
{
    Vector3 contactTangent[ 2 ];

    // Axe Z plus proche de l'axe X ou Y ?
    if( abs( m_contactNormal.x ) > abs( m_contactNormal.y ) )
    {
        // Facteur de mise à l'échelle pour la normalisation
        const float s = ( float ) 1.0f / sqrtf( m_contactNormal.z * m_contactNormal.z +
                                                  m_contactNormal.x * m_contactNormal.x );

        contactTangent[ 0 ].x = m_contactNormal.z * s;
        contactTangent[ 0 ].y = 0;
        contactTangent[ 0 ].z = -m_contactNormal.x * s;

        contactTangent[ 1 ].x = m_contactNormal.y * contactTangent[ 0 ].x;
        contactTangent[ 1 ].y = m_contactNormal.z * contactTangent[ 0 ].x -
            m_contactNormal.x * contactTangent[ 0 ].z;
        contactTangent[ 1 ].z = -m_contactNormal.y * contactTangent[ 0 ].x;
    }
    else
    {
        // Facteur de mise à l'échelle pour la normalisation
        const float s = ( float ) 1.0 / sqrtf( m_contactNormal.z * m_contactNormal.z +
                                                 m_contactNormal.y * m_contactNormal.y );

        contactTangent[ 0 ].x = 0;
        contactTangent[ 0 ].y = -m_contactNormal.z * s;
        contactTangent[ 0 ].z = m_contactNormal.y * s;

        contactTangent[ 1 ].x = m_contactNormal.y * contactTangent[ 0 ].z -
            m_contactNormal.z * contactTangent[ 0 ].y;
        contactTangent[ 1 ].y = -m_contactNormal.x * contactTangent[ 0 ].z;
        contactTangent[ 1 ].z = m_contactNormal.x * contactTangent[ 0 ].y;
    }

    // Matrice à partir des trois vecteurs
    contactToWorld.setComponents(
        m_contactNormal,
        contactTangent[ 0 ],
        contactTangent[ 1 ] );
}

void Contact::applyVelocityChange( Vector3 velocityChange[ 2 ], Vector3 rotationChange[ 2 ] )
{
    // Get hold of the inverse mass and inverse inertia tensor, both in
    // world coordinates.
    Matrix3x3 inverseInertiaTensor[ 2 ];
    m_rigidbodies[ 0 ]->getInverseInertiaTensorWorld( &inverseInertiaTensor[ 0 ] );
    if( m_rigidbodies[ 1 ] )
        m_rigidbodies[ 1 ]->getInverseInertiaTensorWorld( &inverseInertiaTensor[ 1 ] );

    // We will calculate the impulse for each contact axis
    Vector3 impulseContact;

    if( m_friction == ( float ) 0.0 )
    {
        // Use the short format for frictionless contacts
        impulseContact = calculateFrictionlessImpulse( inverseInertiaTensor );
    }
    else
    {
        // Otherwise we may have impulses that aren't in the direction of the
        // contact, so we need the more complex version.
        impulseContact = calculateFrictionImpulse( inverseInertiaTensor );
    }

    // Convert impulse to world coordinates
    Vector3 impulse = contactToWorld.transform( impulseContact );

    // Split in the impulse into linear and rotational components
    Vector3 impulsiveTorque = relativeContactPosition[ 0 ] * impulse;
    rotationChange[ 0 ] = inverseInertiaTensor[ 0 ].transform( impulsiveTorque );
    velocityChange[ 0 ].clear();
    velocityChange[ 0 ] += impulse * m_rigidbodies[ 0 ]->getInverseMass();

    // Apply the changes
    m_rigidbodies[ 0 ]->addVelocity( velocityChange[ 0 ] );
    m_rigidbodies[ 0 ]->addRotation( rotationChange[ 0 ] );

    if( m_rigidbodies[ 1 ] )
    {
        // Work out body one's linear and angular changes
        Vector3 impulsiveTorque = impulse * relativeContactPosition[ 1 ];
        rotationChange[ 1 ] = inverseInertiaTensor[ 1 ].transform( impulsiveTorque );
        velocityChange[ 1 ].clear();
        velocityChange[ 1 ] += impulse * -m_rigidbodies[ 1 ]->getInverseMass();

        // And apply them.
        m_rigidbodies[ 1 ]->addVelocity( velocityChange[ 1 ] );
        m_rigidbodies[ 1 ]->addRotation( rotationChange[ 1 ] );
    }
}

void Contact::applyPositionChange( Vector3 linearChange[ 2 ], Vector3 angularChange[ 2 ], float penetration )
{
    const float angularLimit = ( float ) 0.2f;
    float angularMove[ 2 ];
    float linearMove[ 2 ];

    float totalInertia = 0;
    float linearInertia[ 2 ];
    float angularInertia[ 2 ];

    // We need to work out the inertia of each object in the direction
    // of the contact normal, due to angular inertia only.
    for( unsigned i = 0; i < 2; i++ ) if( m_rigidbodies[ i ] )
    {
        Matrix3x3 inverseInertiaTensor;
        m_rigidbodies[ i ]->getInverseInertiaTensorWorld( &inverseInertiaTensor );

        // Use the same procedure as for calculating frictionless
        // velocity change to work out the angular inertia.
        Vector3 angularInertiaWorld =
            relativeContactPosition[ i ] * m_contactNormal;
        angularInertiaWorld =
            inverseInertiaTensor.transform( angularInertiaWorld );
        angularInertiaWorld =
            angularInertiaWorld * relativeContactPosition[ i ];
        angularInertia[ i ] =
            angularInertiaWorld.dotProduct( m_contactNormal );

        // The linear component is simply the inverse mass
        linearInertia[ i ] = m_rigidbodies[ i ]->getInverseMass();

        // Keep track of the total inertia from all components
        totalInertia += linearInertia[ i ] + angularInertia[ i ];

        // We break the loop here so that the totalInertia value is
        // completely calculated (by both iterations) before
        // continuing.
    }

    // Loop through again calculating and applying the changes
    for( unsigned i = 0; i < 2; i++ ) if( m_rigidbodies[ i ] )
    {
        // The linear and angular movements required are in proportion to
        // the two inverse inertias.
        float sign = ( i == 0 ) ? 1 : -1;
        angularMove[ i ] =
            sign * penetration * ( angularInertia[ i ] / totalInertia );
        linearMove[ i ] =
            sign * penetration * ( linearInertia[ i ] / totalInertia );

        // To avoid angular projections that are too great (when mass is large
        // but inertia tensor is small) limit the angular move.
        Vector3 projection = relativeContactPosition[ i ];
        projection += m_contactNormal * -relativeContactPosition[ i ].dotProduct( m_contactNormal );

        // Use the small angle approximation for the sine of the angle (i.e.
        // the magnitude would be sine(angularLimit) * projection.magnitude
        // but we approximate sine(angularLimit) to angularLimit).
        float maxMagnitude = angularLimit * projection.norm();

        if( angularMove[ i ] < -maxMagnitude )
        {
            float totalMove = angularMove[ i ] + linearMove[ i ];
            angularMove[ i ] = -maxMagnitude;
            linearMove[ i ] = totalMove - angularMove[ i ];
        }
        else if( angularMove[ i ] > maxMagnitude )
        {
            float totalMove = angularMove[ i ] + linearMove[ i ];
            angularMove[ i ] = maxMagnitude;
            linearMove[ i ] = totalMove - angularMove[ i ];
        }

        // We have the linear amount of movement required by turning
        // the rigid body (in angularMove[i]). We now need to
        // calculate the desired rotation to achieve that.
        if( angularMove[ i ] == 0 )
        {
            // Easy case - no angular movement means no rotation.
            angularChange[ i ].clear();
        }
        else
        {
            // Work out the direction we'd like to rotate in.
            Vector3 targetAngularDirection =
                relativeContactPosition[ i ] * m_contactNormal;

            Matrix3x3 inverseInertiaTensor;
            m_rigidbodies[ i ]->getInverseInertiaTensorWorld( &inverseInertiaTensor );

            // Work out the direction we'd need to rotate to achieve that
            angularChange[ i ] =
                inverseInertiaTensor.transform( targetAngularDirection ) *
                ( angularMove[ i ] / angularInertia[ i ] );
        }

        // Velocity change is easier - it is just the linear movement
        // along the contact normal.
        linearChange[ i ] = m_contactNormal * linearMove[ i ];

        // Now we can start to apply the values we've calculated.
        // Apply the linear movement
        Vector3 pos;
        m_rigidbodies[ i ]->getPosition( &pos );
        pos += m_contactNormal * linearMove[ i ];
        m_rigidbodies[ i ]->setPosition( pos );

        // And the change in orientation
        Quaternion q;
        m_rigidbodies[ i ]->getOrientation( &q );
        q.applyAngularVelocity( angularChange[ i ], ( ( float ) 1.0 ) );
        m_rigidbodies[ i ]->setOrientation( q );

        // We need to calculate the derived data for any body that is
        // asleep, so that the changes are reflected in the object's
        // data. Otherwise the resolution will not change the position
        // of the object, and the next collision detection round will
        // have the same penetration.
        m_rigidbodies[ i ]->calculateDerivedData();
    }
}
