#include "Vector.h"
#include <math.h>

/**
 * @brief Constructeur avec coordonnées
 * @param x 
 * @param y 
 * @param z 
*/
Vector3::Vector3( const float& newX, const float& newY, const float& newZ )
    : x( newX ), y( newY ), z( newZ )
{
}


/**
 * @brief Constructeur par copie
 * @param vector 
*/
Vector3::Vector3( const Vector3& vector )
    : Vector3( vector.x, vector.y, vector.z )
{
}


/**
* @brief produit par un scalaire
* @param float
* @return
*/
Vector3 Vector3::operator*( const float& value ) const
{
    return Vector3( x * value, y * value, z * value );
}

/**
* @brief produit par un scalaire
* @param float
* @return
*/
Vector3& Vector3::operator*=( const float& value )
{
    *this = *this * value;

    return *this;
}

/**
 * @brief Division par un scalaire
 * @param value 
 * @return 
*/
Vector3 Vector3::operator/( const float& value ) const
{
    return Vector3( x / value, y / value, z / value );
}


/**
 * @brief Division par un scalaire
 * @param value 
 * @return 
*/
Vector3& Vector3::operator/=( const float& value )
{
    *this = *this / value;

    return *this;
}

/**
 * @brief Produit vectoriel (que pour des vecteurs 3D)
 * @param vector
 * @return
*/
Vector3 Vector3::operator*( const Vector3& vector ) const
{
    return Vector3( y * vector.z - z * vector.y,
                    z * vector.x - x * vector.z,
                    x * vector.y - y * vector.x );
}

/**
 * @brief Produit scalaire
 * @param vector
 * @return
*/
float Vector3::dotProduct( const Vector3& vector ) const
{
    return x * vector.x + y * vector.y + z * vector.z;
}

/**
* @brief addition de deux vecteurs
* @param vector
* @return
*/
Vector3 Vector3::operator+( const Vector3& vector ) const
{
    return Vector3( x + vector.x, y + vector.y, z + vector.z );
}

/**
* @brief Surcharge de l'addition
* @param vector
* @return
*/
Vector3& Vector3::operator+=(const Vector3& vector)
{
    *this = *this + vector;

    return *this;
}



Vector3 Vector3::operator-() const
{
    return *this * -1;
}

Vector3 Vector3::operator-( const Vector3& vector ) const
{
    return *this + ( -vector );
}

Vector3& Vector3::operator-=( const Vector3& vector )
{
    *this = *this - vector;

    return *this;
}

/**
* @brief Surcharge de l'operateur =
* @param vector
* @return
*/
Vector3& Vector3::operator=(const Vector3& vector)
{
    x = vector.x;
    y = vector.y;
    z = vector.z;

    return *this;
}


/**
* @brief Test d'égalité
* @param vector
* @return
*/
bool Vector3::operator==(const Vector3& vector) const
{
    bool isEqual = false;

    if (x == vector.x &&
        y == vector.y &&
        z == vector.z)
    {
        isEqual = true;
    }

    return isEqual;
}

bool Vector3::operator!=(const Vector3& vector) const
{
    return !(*this == vector);
}

void Vector3::clear()
{
    x = y = z = 0;
}

/**
* @brief calcul de la norme
*
*/
float Vector3::norm() const
{
    return sqrt( x * x +
                 y * y +
                 z * z );
}


/**
 * @brief Renvoie le vecteur en version normalisée
 * @return 
*/
Vector3 Vector3::normalized() const
{
    Vector3 resultVector( *this );
    float vectorNorm = norm();

    if( vectorNorm > 0 )
    {
        resultVector /= vectorNorm;
    }

    return resultVector;
}

/**
* @brief Normalisation du vecteur
*/
void Vector3::normalize()
{
    // Réutilisation de code pour réduire la redondance malgré une légère perte de performance
    *this = normalized();
}


/**
 * @brief Calcule la distance entre deux vecteurs
 * @param vector 
 * @return 
*/
float Vector3::distance( const Vector3& vector ) const
{
    return sqrt( pow( vector.x - x, 2 ) +
                 pow( vector.y - y, 2 ) +
                 pow( vector.z - z, 2 ) );
}


/**
 * @brief Affiche le vecteur dans le terminal
 * @param out 
*/
void Vector3::show( std::ostream& out ) const
{
    out << "(" << x << ", " << y << ", " << z << ")" << '\n';
}