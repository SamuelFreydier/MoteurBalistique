#include "Rigidbody.h"
#include "Engine.h"

Rigidbody::Rigidbody( const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& color )
    : m_velocity( velocity ), m_position( position ), m_color( color )
{
    setMassReverse(mass);
}

Rigidbody::Rigidbody( const Rigidbody& rigidbody )
    : Rigidbody(rigidbody.getMass(), rigidbody.getVelocity(), rigidbody.getPosition(), rigidbody.getColor() )
{
    m_acceleration = rigidbody.m_acceleration;
    m_lastFrameAcceleration = rigidbody.m_lastFrameAcceleration;
    m_accumForce = rigidbody.m_accumForce;
    m_accumTorque = rigidbody.m_accumTorque;
}

/**
 * @brief Met à jour l'inverse de la masse en fonction de la masse
 * @param mass 
*/
void Rigidbody::setMassReverse( const float& mass )
{
    if( mass == 0 )
    {
        m_inverseMass = std::numeric_limits<float>::max();
    }
    else
    {
        m_inverseMass = 1 / mass;
    }
}

/**
 * @brief Met à jour le tenseur d'inertie inverse du corps rigide
 * @param inertiaTensor 
*/
void Rigidbody::setInertiaTensor( const Matrix3x3& inertiaTensor )
{
    m_inverseInertiaTensor = inertiaTensor.inverted();
}

/**
 * @brief Convertis les coordonnées du point de local (objet) à monde
 * @param point 
 * @return 
*/
Vector3 Rigidbody::getPointInLocalSpace( const Vector3& point ) const
{
    return m_transformMatrix.transformInverse( point );
}


/**
 * @brief Convertis les coordonnées du point de monde à local (objet)
 * @param point 
 * @return 
*/
Vector3 Rigidbody::getPointInWorldSpace( const Vector3& point ) const
{
    return m_transformMatrix.transform( point );
}

/**
 * @brief Ajoute la force forceVector au vecteur d'accumulation m_accumForce
 * @param forceVector
*/
void Rigidbody::addForce( const Vector3& forceVector )
{
    m_accumForce += forceVector;
}

/**
 * @brief Ajoute une certaine force à un certain point du corps
 * @param force : coordonnées monde
 * @param point : coordonnées objet
*/
void Rigidbody::addForceAtBodyPoint( const Vector3& force, const Vector3& point )
{
    Vector3 pt = getPointInWorldSpace( point );
    addForceAtPoint( force, pt );
}


/**
 * @brief Ajoute une certaine force à un certain point du corps
 * @param force : coordonnées monde
 * @param point : coordonnées monde
*/
void Rigidbody::addForceAtPoint( const Vector3& force, const Vector3& point )
{
    // Conversion en coordonnées relatives au centre de masse
    Vector3 lever = point;
    lever -= m_position;

    m_accumForce += force;

    // Produit vectoriel pour obtenir le torque à partir de la force et du levier
    m_accumTorque += lever * force;
}

/**
 * @brief Nettoie les vecteurs d'accumulation de force et de torque
*/
void Rigidbody::clearAccum()
{
    m_accumForce.clear();
    m_accumTorque.clear();
}

/**
 * @brief Calcule des données plus pratiques à manipuler dans le corps rigide (redondance avec les données déjà existantes)
*/
void Rigidbody::calculateDerivedData()
{
    m_orientation.normalize();

    // Calcul de la matrice de transformation pour le corps
    calculateTransformMatrix( m_transformMatrix, m_position, m_orientation );

    // Calcul du tenseur d'inertie en coordonnées monde
    transformInertiaTensor( m_inverseInertiaTensorWorld, m_orientation, m_inverseInertiaTensor, m_transformMatrix );
}

/**
 * @brief Calcule une matrice de transformation à partir d'une position et d'une orientation
 * @param transformMatrix : output
 * @param position 
 * @param orientation 
*/
void Rigidbody::calculateTransformMatrix( Matrix4x4& transformMatrix, const Vector3& position, const Quaternion& orientation )
{
    // On ne considère que la partie "rotation" de la matrice et non pas la translation => Matrice 4x3 = 12 éléments
    transformMatrix[ 0 ] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
    transformMatrix[ 1 ] = 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k;
    transformMatrix[ 2 ] = 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j;
    transformMatrix[ 3 ] = position.x;
    transformMatrix[ 4 ] = 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k;
    transformMatrix[ 5 ] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k;
    transformMatrix[ 6 ] = 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i;
    transformMatrix[ 7 ] = position.y;
    transformMatrix[ 8 ] = 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j;
    transformMatrix[ 9 ] = 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i;
    transformMatrix[ 10 ] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j;
    transformMatrix[ 11 ] = position.z;
}

/**
 * @brief Effectue la transformation en coordonnées monde sur un tenseur d'inertie grâce à un quaternion
 * @param inverseInertiaTensorWorld 
 * @param q 
 * @param inverseInertiaTensorBody 
 * @param rotationMatrix 
*/
void Rigidbody::transformInertiaTensor( Matrix3x3& inverseInertiaTensorWorld, const Quaternion& q, const Matrix3x3& inverseInertiaTensorBody, const Matrix4x4& rotationMatrix )
{
    float line0col0 = rotationMatrix[ 0 ] * inverseInertiaTensorBody[ 0 ] + rotationMatrix[ 1 ] * inverseInertiaTensorBody[ 3 ] + rotationMatrix[ 2 ] * inverseInertiaTensorBody[ 6 ];
    float line0col1 = rotationMatrix[ 0 ] * inverseInertiaTensorBody[ 1 ] + rotationMatrix[ 1 ] * inverseInertiaTensorBody[ 4 ] + rotationMatrix[ 2 ] * inverseInertiaTensorBody[ 7 ];
    float line0col2 = rotationMatrix[ 0 ] * inverseInertiaTensorBody[ 2 ] + rotationMatrix[ 1 ] * inverseInertiaTensorBody[ 5 ] + rotationMatrix[ 2 ] * inverseInertiaTensorBody[ 8 ];
    float line1col0 = rotationMatrix[ 4 ] * inverseInertiaTensorBody[ 0 ] + rotationMatrix[ 5 ] * inverseInertiaTensorBody[ 3 ] + rotationMatrix[ 6 ] * inverseInertiaTensorBody[ 6 ];
    float line1col1 = rotationMatrix[ 4 ] * inverseInertiaTensorBody[ 1 ] + rotationMatrix[ 5 ] * inverseInertiaTensorBody[ 4 ] + rotationMatrix[ 6 ] * inverseInertiaTensorBody[ 7 ];
    float line1col2 = rotationMatrix[ 4 ] * inverseInertiaTensorBody[ 2 ] + rotationMatrix[ 5 ] * inverseInertiaTensorBody[ 5 ] + rotationMatrix[ 6 ] * inverseInertiaTensorBody[ 8 ];
    float line2col0 = rotationMatrix[ 8 ] * inverseInertiaTensorBody[ 0 ] + rotationMatrix[ 9 ] * inverseInertiaTensorBody[ 3 ] + rotationMatrix[ 10 ] * inverseInertiaTensorBody[ 6 ];
    float line2col1 = rotationMatrix[ 8 ] * inverseInertiaTensorBody[ 1 ] + rotationMatrix[ 9 ] * inverseInertiaTensorBody[ 4 ] + rotationMatrix[ 10 ] * inverseInertiaTensorBody[ 7 ];
    float line2col2 = rotationMatrix[ 8 ] * inverseInertiaTensorBody[ 2 ] + rotationMatrix[ 9 ] * inverseInertiaTensorBody[ 5 ] + rotationMatrix[ 10 ] * inverseInertiaTensorBody[ 8 ];

    inverseInertiaTensorWorld[ 0 ] = line0col0 * rotationMatrix[ 0 ] +
        line0col1 * rotationMatrix[ 1 ] +
        line0col2 * rotationMatrix[ 2 ];
    inverseInertiaTensorWorld[ 1 ] = line0col0 * rotationMatrix[ 4 ] +
        line0col1 * rotationMatrix[ 5 ] +
        line0col2 * rotationMatrix[ 6 ];
    inverseInertiaTensorWorld[ 2 ] = line0col0 * rotationMatrix[ 8 ] +
        line0col1 * rotationMatrix[ 9 ] +
        line0col2 * rotationMatrix[ 10 ];
    inverseInertiaTensorWorld[ 3 ] = line1col0 * rotationMatrix[ 0 ] +
        line1col1 * rotationMatrix[ 1 ] +
        line1col2 * rotationMatrix[ 2 ];
    inverseInertiaTensorWorld[ 4 ] = line1col0 * rotationMatrix[ 4 ] +
        line1col1 * rotationMatrix[ 5 ] +
        line1col2 * rotationMatrix[ 6 ];
    inverseInertiaTensorWorld[ 5 ] = line1col0 * rotationMatrix[ 8 ] +
        line1col1 * rotationMatrix[ 9 ] +
        line1col2 * rotationMatrix[ 10 ];
    inverseInertiaTensorWorld[ 6 ] = line2col0 * rotationMatrix[ 0 ] +
        line2col1 * rotationMatrix[ 1 ] +
        line2col2 * rotationMatrix[ 2 ];
    inverseInertiaTensorWorld[ 7 ] = line2col0 * rotationMatrix[ 4 ] +
        line2col1 * rotationMatrix[ 5 ] +
        line2col2 * rotationMatrix[ 6 ];
    inverseInertiaTensorWorld[ 8 ] = line2col0 * rotationMatrix[ 8 ] +
        line2col1 * rotationMatrix[ 9 ] +
        line2col2 * rotationMatrix[ 10 ];
}


/**
 * @brief Calcul de la nouvelle vélocité et de la nouvelle position à partir de l'accélération via l'intégration d'Euler (linéaire + angulaire)
 * @param deltaTime
*/
void Rigidbody::integrate( const float& secondsElapsedSincePreviousUpdate)
{
    // Accélération linéaire
    m_lastFrameAcceleration = m_acceleration;
    m_lastFrameAcceleration = m_accumForce * getInverseMass();
    
    // Accélération angulaire
    Vector3 angularAcceleration = m_inverseInertiaTensorWorld.transform( m_accumTorque );

    // Vélocité linéaire
    m_velocity = m_velocity * pow( m_linearDamping, secondsElapsedSincePreviousUpdate ) + m_acceleration * secondsElapsedSincePreviousUpdate;
            
    // Vélocité angulaire
    m_angularVelocity = m_angularVelocity * pow( m_angularDamping, secondsElapsedSincePreviousUpdate ) * angularAcceleration * secondsElapsedSincePreviousUpdate;

    // Position
    m_position += m_velocity * secondsElapsedSincePreviousUpdate;

    // Orientation
    m_orientation.applyAngularVelocity( m_angularVelocity, secondsElapsedSincePreviousUpdate );

    // Normalise l'orientation + Met à jour les matrices avec les nouvelles données
    calculateDerivedData();

    // Nettoyage du vecteur d'accumulation de forces et de torque
    clearAccum();
}