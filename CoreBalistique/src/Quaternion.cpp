#include "Quaternion.h"
#include <math.h>

/**
 * @brief Constructeur créant un quaternion de rotation 0 par défaut
 * @param newR 
 * @param newI 
 * @param newJ 
 * @param newK 
*/
Quaternion::Quaternion( const float& newR, const float& newI, const float& newJ, const float& newK )
    : r( newR ), i( newI ), j( newJ ), k( newK )
{
}

/**
 * @brief Normalise le quaternion pour que sa norme soit de 1 et qu'il devienne un quaternion de rotation valide
*/
void Quaternion::normalize()
{
    float norm = getNorm();

    // Si la norme est égale à 0, on met la partie réelle à 1 (aucune rotation)
    if( norm == 0 )
    {
        r = 1;
        return;
    }

    r /= norm;
    i /= norm;
    j /= norm;
    k /= norm;
}

/**
 * @brief Norme du quaternion
 * @return 
*/
float Quaternion::getNorm() const
{
    return sqrt( r * r + i * i + j * j + k * k );
}

/**
 * @brief Conjugué du quaternion
 * @return 
*/
Quaternion Quaternion::getConjugate() const
{
    Quaternion NewQuaternion( *this );

    NewQuaternion.i *= -1;
    NewQuaternion.j *= -1;
    NewQuaternion.k *= -1;
    
    return NewQuaternion;
}

/**
 * @brief Inverse du quaternion
 * @return 
*/
Quaternion Quaternion::getInverse() const
{
    return getConjugate() / getNorm();
}

Quaternion Quaternion::getExponentiation( const float& fraction )
{
    Quaternion NewQuaternion( *this );

    float alpha = acos( r );
    float sinFractionAlpha = sin( fraction * alpha );
    float sinAlpha = sin( alpha );

    NewQuaternion.r = cos( fraction * alpha );
    NewQuaternion.i *= sinFractionAlpha / sinAlpha;
    NewQuaternion.j *= sinFractionAlpha / sinAlpha;
    NewQuaternion.k *= sinFractionAlpha / sinAlpha;

    return NewQuaternion;
}

/**
 * @brief Interpolation linéaire sphérique (Spherical linear interpolation) entre le quaternion courant et un quaternion de destination en utilisant l'exponentiation
 * @param destination 
 * @param fraction 
 * @return 
*/
Quaternion Quaternion::slerp( const Quaternion& destination, const float& fraction )
{
    Quaternion deltaQuaternion( destination * getInverse() );

    return deltaQuaternion.getExponentiation( fraction ) * ( *this );
}

/**
 * @brief Multiplication entre deux quaternions
 * @param other 
 * @return 
*/
Quaternion Quaternion::operator*( const Quaternion& other ) const
{
    Quaternion NewQuaternion( *this );

    NewQuaternion.r = r * other.r - i * other.i - j * other.j - k * other.k;
    NewQuaternion.i = r * other.i + i * other.r + j * other.k - k * other.j;
    NewQuaternion.j = r * other.j + j * other.r + k * other.i - i * other.k;
    NewQuaternion.k = r * other.k + k * other.r + i * other.j - j * other.i;

    return NewQuaternion;
}

/**
 * @brief Multiplication entre deux quaternions
 * @param other 
 * @return 
*/
Quaternion& Quaternion::operator*=( const Quaternion& other )
{
    *this = ( *this ) * other;

    return *this;
}

/**
 * @brief Négation du quaternion
 * @return 
*/
Quaternion Quaternion::operator-() const
{
    return ( *this ) * -1;
}

/**
 * @brief Produit scalaire entre deux quaternions
 * @param other 
 * @return 
*/
float Quaternion::dotProduct( const Quaternion& other ) const
{
    return r * other.r + i * other.i + j * other.j + k * other.k;
}

/**
 * @brief Produit du quaternion par un scalaire
 * @param scalar 
 * @return 
*/
Quaternion Quaternion::operator*( const float& scalar ) const
{
    Quaternion NewQuaternion( *this );

    NewQuaternion.r *= scalar;
    NewQuaternion.i *= scalar;
    NewQuaternion.j *= scalar;
    NewQuaternion.k *= scalar;

    return NewQuaternion;
}


/**
 * @brief Produit du quaternion par un scalaire
 * @param scalar
 * @return
*/
Quaternion& Quaternion::operator*=( const float& scalar )
{
    ( *this ) = ( *this ) * scalar;

    return *this;
}


/**
 * @brief Division du quaternion par un scalaire
 * @param scalar 
 * @return 
*/
Quaternion Quaternion::operator/( const float& scalar ) const
{
    if( scalar == 0 )
    {
        return Quaternion();
    }

    Quaternion NewQuaternion( *this );

    NewQuaternion.r /= scalar;
    NewQuaternion.i /= scalar;
    NewQuaternion.j /= scalar;
    NewQuaternion.k /= scalar;

    return NewQuaternion;
}

/**
 * @brief Division du quaternion par un scalaire
 * @param scalar
 * @return
*/
Quaternion& Quaternion::operator/=( const float& scalar )
{
    ( *this ) = ( *this ) / scalar;

    return *this;
}

/**
 * @brief Met à jour la rotation en fonction d'une vélocité angulaire et du temps. g'(t) = g0 + 1/2 * w * g(t) * deltaTime
 * @param angularVelocity : vélocité angulaire <=> vecteur w calculé au préalable à partir de la fréquence angulaire (vitesse / rayon)
 * @param duration : montant du vecteur à ajouter
*/
void Quaternion::applyAngularVelocity( const Vector3& angularVelocity, float deltaTime )
{
    // Quaternion de vélocité angulaire omega
    Quaternion w( 0, angularVelocity.x, angularVelocity.y, angularVelocity.z );
   
    // calculIntermediaire = 1/2 * w * g * deltaTime
    w *= 0.5f;
    w *= ( *this );
    w *= deltaTime;

    // g' = g + calculIntermediare
    r += w.r;
    i += w.i;
    j += w.j;
    k += w.k;
}
