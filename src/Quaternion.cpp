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
    float length = r * r + i * i + j * j + k * k;

    // Si la norme est égale à 0, on met la partie réelle à 1 (aucune rotation)
    if( length == 0 )
    {
        r = 1;
        return;
    }

    length = 1.0f / sqrt( length );
    r *= length;
    i *= length;
    j *= length;
    k *= length;
}

/**
 * @brief Multiplication entre deux quaternions
 * @param other 
 * @return 
*/
Quaternion Quaternion::operator*( const Quaternion& other )
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
 * @brief Tourne le quaternion d'un certain montant, indiqué par un vecteur
 * @param vector 
*/
void Quaternion::rotateByVector( const Vector3& vector )
{
    Quaternion NewQuaternion( 0, vector.x, vector.y, vector.z );
    ( *this ) *= NewQuaternion;
}

/**
 * @brief Met à jour la rotation avec un vecteur et une échelle. Permet de mettre à jour le quaternion avec une nouvelle rotation et le temps
 * @param vector : vecteur à ajouter
 * @param scale : montant du vecteur à ajouter
*/
void Quaternion::addScaledVector( const Vector3& vector, float scale )
{
    Quaternion NewQuaternion( 0, vector.x * scale, vector.y * scale, vector.z * scale );
   
    NewQuaternion *= ( *this );

    r += NewQuaternion.r * 0.5f;
    i += NewQuaternion.i * 0.5f;
    j += NewQuaternion.j * 0.5f;
    k += NewQuaternion.k * 0.5f;
}
