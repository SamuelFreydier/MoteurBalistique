#include "ParticleContact.h"

/**
 * @brief Calcule l'impulsion n�cessaire pour la r�solution de la collision, sans prendre en compte la friction
 * @param inverseInertiaTensor 
 * @return 
*/
Vector3 Contact::calculateFrictionlessImpulse( Matrix3x3* inverseInertiaTensor )
{
    Vector3 impulseContact;

    // Vecteur contenant le changement en v�locit� en coordonn�es monde pour une impulsion unitaire dans la direction de la normale
    Vector3 deltaVelWorld = relativeContactPosition[ 0 ] * m_contactNormal;
    deltaVelWorld = inverseInertiaTensor[ 0 ].transform( deltaVelWorld );
    deltaVelWorld = deltaVelWorld * relativeContactPosition[ 0 ];

    // Changement de v�locit� en coordonn�es de la collision
    float deltaVelocity = deltaVelWorld.dotProduct( m_contactNormal );

    // Ajout de la partie lin�aire du changement de v�locit�
    deltaVelocity += m_rigidbodies[ 0 ]->getInverseMass();

    // Idem sur l'autre corps
    if( m_rigidbodies[ 1 ] )
    {
        Vector3 deltaVelWorld = relativeContactPosition[ 1 ] * m_contactNormal;
        deltaVelWorld = inverseInertiaTensor[ 1 ].transform( deltaVelWorld );
        deltaVelWorld = deltaVelWorld * relativeContactPosition[ 1 ];

        deltaVelocity += deltaVelWorld.dotProduct( m_contactNormal );

        deltaVelocity += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Quantit� d'impulsion
    impulseContact.x = desiredDeltaVelocity / deltaVelocity;
    impulseContact.y = 0;
    impulseContact.z = 0;
    return impulseContact;
}

Vector3 Contact::calculateFrictionImpulse( Matrix3x3* inverseInertiaTensor )
{
    Vector3 impulseContact;
    float inverseMass = m_rigidbodies[ 0 ]->getInverseMass();

    // Equivalent d'un produit vectoriel => Multiplication par une matrice antisym�trique
    Matrix3x3 impulseToTorque;
    impulseToTorque.setSkewSymmetric( relativeContactPosition[ 0 ] );

    // Matrice permettant de convertir l'impulsion au contact en changement de v�locit� en coordonn�es monde
    Matrix3x3 deltaVelWorld = impulseToTorque;
    deltaVelWorld *= inverseInertiaTensor[ 0 ];
    deltaVelWorld *= impulseToTorque;
    deltaVelWorld *= -1;

    // Idem pour l'autre corps
    if( m_rigidbodies[ 1 ] )
    {
        impulseToTorque.setSkewSymmetric( relativeContactPosition[ 1 ] );

        Matrix3x3 deltaVelWorld2 = impulseToTorque;
        deltaVelWorld2 *= inverseInertiaTensor[ 1 ];
        deltaVelWorld2 *= impulseToTorque;
        deltaVelWorld2 *= -1;

        deltaVelWorld += deltaVelWorld2;

        inverseMass += m_rigidbodies[ 1 ]->getInverseMass();
    }

    // Changement de base => Rep�re de la collision
    Matrix3x3 deltaVelocity = contactToWorld.transposed();
    deltaVelocity *= deltaVelWorld;
    deltaVelocity *= contactToWorld;

    // Ajout du composant lin�aire du changement de v�locit�
    deltaVelocity[ 0 ] += inverseMass;
    deltaVelocity[ 4 ] += inverseMass;
    deltaVelocity[ 8 ] += inverseMass;

    // Inversion pour avoir l'impulsion n�cessaire par unit� de v�locit�
    Matrix3x3 impulseMatrix = deltaVelocity.inverted();

    // On veut trouver les v�locit�s � �liminer
    Vector3 velKill( desiredDeltaVelocity,
                     -contactVelocity.y,
                     -contactVelocity.z );

    // Impulsion n�cessaire pour supprimer ces v�locit�s
    impulseContact = impulseMatrix.transform( velKill );

    // V�rification de friction exc�dentaire (statique)
    float planarImpulse = sqrtf(
        impulseContact.y * impulseContact.y +
        impulseContact.z * impulseContact.z
    );
    if( planarImpulse > impulseContact.x * m_friction )
    {
        // Friction dynamique
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

float Contact::getRestitution() const
{
    return m_restitution;
}

/**
 * @brief D�finis les donn�es qui ne d�pendent pas de la position de la collision (le tableau des corps, la friction, la restitution)
 * @param one 
 * @param two 
 * @param friction 
 * @param restitution 
*/
void Contact::setBodyData( std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two, float friction, float restitution )
{
    m_rigidbodies[ 0 ] = one;
    m_rigidbodies[ 1 ] = two;
    m_friction = friction;
    m_restitution = restitution;
}


/**
 * @brief Calcule des donn�es internes pour pr�parer la collision. Est appel� avant la r�solution de la collision.
 * @param duration 
*/
void Contact::calculateInternals( float duration )
{
    if( !m_rigidbodies[ 0 ] ) swapBodies();

    // Calcul du rep�re de la collision
    calculateContactBasis();

    // Position relative du point d'impact par rapport � chaque corps
    relativeContactPosition[ 0 ] = m_contactPoint - m_rigidbodies[ 0 ]->getPosition();
    if( m_rigidbodies[ 1 ] ) {
        relativeContactPosition[ 1 ] = m_contactPoint - m_rigidbodies[ 1 ]->getPosition();
    }

    // V�locit� relative des corps � la collision
    contactVelocity = calculateLocalVelocity( 0, duration );
    if( m_rigidbodies[ 1 ] ) {
        contactVelocity -= calculateLocalVelocity( 1, duration );
    }

    // Calcul du changement voulu de v�locit� pour la r�solution future
    calculateDesiredDeltaVelocity( duration );
}


/**
 * @brief Echange les deux corps de la collision
*/
void Contact::swapBodies()
{
    m_contactNormal *= -1;

    std::shared_ptr<Rigidbody> temp = m_rigidbodies[ 0 ];
    m_rigidbodies[ 0 ] = m_rigidbodies[ 1 ];
    m_rigidbodies[ 1 ] = temp;
}


/**
 * @brief Calcule le changement de v�locit� voulu pour la r�solution
 * @param duration 
*/
void Contact::calculateDesiredDeltaVelocity( float duration )
{
    const static float velocityLimit = ( float ) 0.25f;

    // Calcul de la v�locit� induite par l'acc�l�ration sur cette frame
    float velocityFromAcc = 0;

    velocityFromAcc +=
        (m_rigidbodies[ 0 ]->getAcceleration() * duration).dotProduct(m_contactNormal);

    if( m_rigidbodies[ 1 ] )
    {
        velocityFromAcc -=
            (m_rigidbodies[ 1 ]->getAcceleration() * duration).dotProduct(m_contactNormal);
    }

    // Si la v�locit� est trop basse, on limite la restitution
    float thisRestitution = m_restitution;
    if( abs( contactVelocity.x ) < velocityLimit )
    {
        thisRestitution = ( float ) 0.0f;
    }

    // On enl�ve la v�locit� due � l'acc�l�ration et on applique la restitution
    desiredDeltaVelocity =
        -contactVelocity.x
        - thisRestitution * ( contactVelocity.x - velocityFromAcc );
}


/**
 * @brief Calcule et retourne la v�locit� au point de contact qui est appliqu�e sp�cifiquement sur un des deux corps
 * @param bodyIndex 
 * @param duration 
 * @return 
*/
Vector3 Contact::calculateLocalVelocity( unsigned bodyIndex, float duration )
{
    std::shared_ptr<Rigidbody> thisBody = m_rigidbodies[ bodyIndex ];

    // Calcule la v�locit� au point de contact
    Vector3 velocity =
        thisBody->getAngularVelocity() * relativeContactPosition[bodyIndex];
    velocity += thisBody->getVelocity();

    // Changement de rep�re => Rep�re de la collision
    Vector3 contactVelocity = contactToWorld.transformTranspose( velocity );

    // Calcul du montant de v�locit� d� � l'acc�l�ration
    Vector3 accVelocity = thisBody->getAcceleration() * duration;

    // Changement de rep�re => Rep�re de la collision
    accVelocity = contactToWorld.transformTranspose( accVelocity );

    // On ignore n'importe quelle acc�l�ration dans la direction de la normale (on ne souhaite que l'acc�l�ration dans le plan de la collision)
    accVelocity.x = 0;

    // Ajout des v�locit�s dans le plan. Elles seront potentiellement supprim�es par la friction plus tard
    contactVelocity += accVelocity;

    return contactVelocity;
}


/**
 * @brief Calcule d'une base orthonorm�e au point de contact (rep�re de la collision)
*/
void Contact::calculateContactBasis()
{
    Vector3 contactTangent[ 2 ];

    // Axe Z plus proche de l'axe X ou Y ?
    if( abs( m_contactNormal.x ) > abs( m_contactNormal.y ) )
    {
        // Facteur de mise � l'�chelle pour la normalisation
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
        // Facteur de mise � l'�chelle pour la normalisation
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

    // Matrice � partir des trois vecteurs
    contactToWorld.setComponents(
        m_contactNormal,
        contactTangent[ 0 ],
        contactTangent[ 1 ] );
}


/**
 * @brief R�solution de la v�locit� (impulsion)
 * @param velocityChange 
 * @param rotationChange 
*/
void Contact::applyVelocityChange( Vector3 velocityChange[ 2 ], Vector3 rotationChange[ 2 ] )
{
    // Calcul de l'inverse de la masse et du tenseur d'inertie inverse, en coordonn�es monde
    Matrix3x3 inverseInertiaTensor[ 2 ];
    m_rigidbodies[ 0 ]->getInverseInertiaTensorWorld( &inverseInertiaTensor[ 0 ] );
    if( m_rigidbodies[ 1 ] )
        m_rigidbodies[ 1 ]->getInverseInertiaTensorWorld( &inverseInertiaTensor[ 1 ] );

    Vector3 impulseContact;

    if( m_friction == ( float ) 0.0 )
    {
        // Pas de friction
        impulseContact = calculateFrictionlessImpulse( inverseInertiaTensor );
    }
    else
    {
        // Version plus complexe avec friction
        impulseContact = calculateFrictionImpulse( inverseInertiaTensor );
    }

    // Impulsion en coordonn�es monde
    Vector3 impulse = contactToWorld.transform( impulseContact );

    // S�paration de l'impulsion en composants lin�aires et angulaires
    Vector3 impulsiveTorque = relativeContactPosition[ 0 ] * impulse;
    rotationChange[ 0 ] = inverseInertiaTensor[ 0 ].transform( impulsiveTorque );
    velocityChange[ 0 ].clear();
    velocityChange[ 0 ] += impulse * m_rigidbodies[ 0 ]->getInverseMass();

    // Application des changements
    m_rigidbodies[ 0 ]->addVelocity( velocityChange[ 0 ] );
    m_rigidbodies[ 0 ]->addRotation( rotationChange[ 0 ] );

    // Idem pour le deuxi�me corps
    if( m_rigidbodies[ 1 ] )
    {
        Vector3 impulsiveTorque = impulse * relativeContactPosition[ 1 ];
        rotationChange[ 1 ] = inverseInertiaTensor[ 1 ].transform( impulsiveTorque );
        velocityChange[ 1 ].clear();
        velocityChange[ 1 ] += impulse * -m_rigidbodies[ 1 ]->getInverseMass();

        m_rigidbodies[ 1 ]->addVelocity( velocityChange[ 1 ] );
        m_rigidbodies[ 1 ]->addRotation( rotationChange[ 1 ] );
    }
}


/**
 * @brief R�solution de l'interp�n�tration
 * @param linearChange 
 * @param angularChange 
 * @param penetration 
*/
void Contact::applyPositionChange( Vector3 linearChange[ 2 ], Vector3 angularChange[ 2 ], float penetration )
{
    const float angularLimit = ( float ) 0.2f;
    float angularMove[ 2 ];
    float linearMove[ 2 ];

    float totalInertia = 0;
    float linearInertia[ 2 ];
    float angularInertia[ 2 ];

    // Il faut trouver l'inertie de tous les corps dans la direction de la normale, d� uniquement � l'inertie angulaire
    for( unsigned i = 0; i < 2; i++ ) if( m_rigidbodies[ i ] )
    {
        Matrix3x3 inverseInertiaTensor;
        m_rigidbodies[ i ]->getInverseInertiaTensorWorld( &inverseInertiaTensor );

        // M�me proc�d� que pour le calcul de la v�locit� SANS friction, ici pour l'inertie angulaire
        Vector3 angularInertiaWorld =
            relativeContactPosition[ i ] * m_contactNormal;
        angularInertiaWorld =
            inverseInertiaTensor.transform( angularInertiaWorld );
        angularInertiaWorld =
            angularInertiaWorld * relativeContactPosition[ i ];
        angularInertia[ i ] =
            angularInertiaWorld.dotProduct( m_contactNormal );

        // Composant lin�aire = Inverse de la masse
        linearInertia[ i ] = m_rigidbodies[ i ]->getInverseMass();

        // Mise � jour de l'inertie totale � partir de tous les composants (lin�aire/angulaire)
        totalInertia += linearInertia[ i ] + angularInertia[ i ];
    }

    // Calcul et Application des changements
    for( unsigned i = 0; i < 2; i++ ) if( m_rigidbodies[ i ] )
    {
        // Les mouvements lin�aires et angulaires requis sont proportionnels respectivement aux deux inerties inverses
        float sign = ( i == 0 ) ? 1 : -1;
        angularMove[ i ] =
            sign * penetration * ( angularInertia[ i ] / totalInertia );
        linearMove[ i ] =
            sign * penetration * ( linearInertia[ i ] / totalInertia );

        // Pour �viter des projections trop grandes, on limite le mouvement rotationnel
        Vector3 projection = relativeContactPosition[ i ];
        projection += m_contactNormal * -relativeContactPosition[ i ].dotProduct( m_contactNormal );

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

        // Calcul du mouvement de rotation d�sir�
        if( angularMove[ i ] == 0 )
        {
            // Pas de mouvement angulaire => Pas de rotation
            angularChange[ i ].clear();
        }
        else
        {
            // On d�termine la direction de la rotation
            Vector3 targetAngularDirection =
                relativeContactPosition[ i ] * m_contactNormal;

            Matrix3x3 inverseInertiaTensor;
            m_rigidbodies[ i ]->getInverseInertiaTensorWorld( &inverseInertiaTensor );

            angularChange[ i ] =
                inverseInertiaTensor.transform( targetAngularDirection ) *
                ( angularMove[ i ] / angularInertia[ i ] );
        }

        // Changement de v�locit� lin�aire
        linearChange[ i ] = m_contactNormal * linearMove[ i ];

        // Application du mouvement lin�aire
        Vector3 pos;
        m_rigidbodies[ i ]->getPosition( &pos );
        pos += m_contactNormal * linearMove[ i ];
        m_rigidbodies[ i ]->setPosition( pos );

        // Application du mouvement rotationnel
        Quaternion q;
        m_rigidbodies[ i ]->getOrientation( &q );
        q.applyAngularVelocity( angularChange[ i ], ( ( float ) 1.0 ) );
        m_rigidbodies[ i ]->setOrientation( q );

        // On sauvegarde les nouvelles donn�es de position et d'orientation du rigidbody
        m_rigidbodies[ i ]->calculateDerivedData();
    }
}
