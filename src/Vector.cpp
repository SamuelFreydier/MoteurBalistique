#include "Vector.h"
#include <math.h>

// constructeurs
Vector::Vector( const std::vector<float>& coordinates )
    : m_dimension( coordinates.size() )
{
    m_coordinates.reserve( m_dimension );
    for( const float& coordinate : coordinates )
    {
        m_coordinates.emplace_back( coordinate );
    }
}

Vector::Vector( const int& dimension )
    : m_dimension( dimension )
{
    m_coordinates.reserve( m_dimension );
    for( int cptDim = 0; cptDim < m_dimension; cptDim++ )
    {
        m_coordinates.emplace_back( 0.0 );
    }
}

/**
* @brief produit par un scalaire
* @param float
* @return
*/
Vector Vector::operator*( const float& value ) const
{
    Vector newVector( m_dimension );

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        newVector[ coordId ] = m_coordinates[ coordId ] * value;
    }

    return newVector;
}

/**
* @brief produit par un scalaire
* @param float
* @return
*/
Vector& Vector::operator*=( const float& value )
{
    *this = *this * value;

    return *this;
}

/**
 * @brief Produit vectoriel (que pour des vecteurs 3D)
 * @param vector
 * @return
*/
Vector Vector::operator*( const Vector& vector ) const
{
    Vector vctResult( 3 );

    if( m_dimension >= 3 && vector.m_dimension >= 3 )
    {
        vctResult[ 0 ] = m_coordinates[ 1 ] * vector[ 2 ] - m_coordinates[ 2 ] * vector[ 1 ];
        vctResult[ 1 ] = m_coordinates[ 2 ] * vector[ 0 ] - m_coordinates[ 0 ] * vector[ 2 ];
        vctResult[ 2 ] = m_coordinates[ 0 ] * vector[ 1 ] - m_coordinates[ 1 ] * vector[ 0 ];
    }

    return vctResult;
}

/**
 * @brief Produit scalaire
 * @param vector
 * @return
*/
float Vector::dotProduct( const Vector& vector ) const
{
    float dotResult = 0.0;

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        dotResult += m_coordinates[ coordId ] * vector[ coordId ];
    }

    return dotResult;
}

/**
* @brief addition de deux vecteurs
* @param vector
* @return
*/
Vector Vector::operator+( const Vector& vector ) const
{
    Vector newVector( m_dimension );

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        newVector[ coordId ] = m_coordinates[ coordId ] + vector[ coordId ];
    }

    return newVector;
}

Vector Vector::operator-() const
{
    return *this * -1;
}

Vector Vector::operator-( const Vector& vector ) const
{
    return *this + ( -vector );
}

/**
* @brief calcul de la norme
*
*/
float Vector::norm() const
{
    float coordSum = 0.0;

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        coordSum += m_coordinates[ coordId ] * m_coordinates[ coordId ];
    }

    return sqrt( coordSum );
}


/**
 * @brief Renvoie le vecteur en version normalisée
 * @return 
*/
Vector Vector::normalized() const
{
    Vector vectNorm = Vector( *this );

    float thisNorm = norm();

    if( thisNorm != 0 )
    {
        for( int i = 0; i < m_dimension; i++ )
        {
            vectNorm[ i ] /= thisNorm;
        }
    }

    return vectNorm;
}

/**
* @brief normalise le vecteur
*/
void Vector::normalize()
{
    // Réutilisation de code pour réduire la redondance malgré une légère perte de performance
    *this = normalized();
}

/**
* @brief surchage de l'operateur egal
* @param vector
* @return
*/
Vector& Vector::operator=( const Vector& vector )
{
    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        m_coordinates[ coordId ] = vector[ coordId ];
    }

    return *this;
}

/**
* @brief surchage de l'addition
* @param vector
* @return
*/
Vector& Vector::operator+=( const Vector& vector )
{
    *this = *this + vector;

    return *this;
}

/**
* @brief Test d'egalite
* @param vector
* @return
*/
bool Vector::operator==( const Vector& vector ) const
{
    bool isEqual = true;

    for( int coordId = 0; coordId < m_dimension && isEqual == true; coordId++ )
    {
        if( m_coordinates[ coordId ] != vector[ coordId ] )
        {
            isEqual = false;
        }
    }

    return isEqual;
}

bool Vector::operator!=( const Vector& vector ) const
{
    return !( *this == vector );
}

/**
 * @brief Calcule la distance entre deux vecteurs
 * @param vector 
 * @return 
*/
float Vector::distance( const Vector& vector ) const
{
    float sum = 0;
    float distance;

    for( int coord = 0; coord < m_dimension; coord++ )
    {
        sum += pow( vector[ coord ] - m_coordinates[ coord ], 2 );
    }

    return sqrt( sum );
}


/**
 * @brief Affiche le vecteur dans le terminal
 * @param out 
*/
void Vector::show( std::ostream& out ) const
{
    out << "(";

    for( int cptCoord = 0; cptCoord < m_dimension; cptCoord++ )
    {
        out << m_coordinates[ cptCoord ];
        if( cptCoord != m_dimension - 1 )
        {
            out << ", ";
        }
    }

    out << ")" << std::endl;
}