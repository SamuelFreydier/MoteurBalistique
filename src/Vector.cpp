#include "Vector.h"
#include <math.h>

Vector::Vector( const std::vector<float>& coordinates )
    : m_dimension(coordinates.size())
{
    m_coordinates.reserve( m_dimension );
    for( const float& coordinate : coordinates )
    {
        m_coordinates.emplace_back( coordinate );
    }
}

Vector::Vector( const size_t& dimension )
    : m_dimension( dimension )
{
    m_coordinates.reserve( m_dimension );
    for( int cptDim = 0; cptDim < m_dimension; cptDim++ ) 
    {
        m_coordinates.emplace_back( 0.0 );
    }
}

Vector Vector::operator*( const float& value ) const
{
    Vector newVector( m_dimension );

    for( int coordId = 0; coordId < m_dimension; coordId++ ) 
    {
        newVector[ coordId ] = m_coordinates[ coordId ] * value;
    }

    return newVector;
}

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

Vector Vector::operator+( const Vector& vector ) const
{
    Vector newVector( m_dimension );

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        newVector[ coordId ] = m_coordinates[ coordId ] + vector[ coordId ];
    }

    return newVector;
}

float Vector::norm() const
{
    float coordSum = 0.0;

    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        coordSum += m_coordinates[ coordId ] * m_coordinates[ coordId ];
    }

    return sqrt( coordSum );
}

void Vector::normalize()
{
    float vctNorm = norm();

    if( vctNorm != 0 )
    {
        for( int coordId = 0; coordId < m_dimension; coordId++ )
        {
            m_coordinates[ coordId ] /= vctNorm;
        }
    }
}

Vector& Vector::operator=( const Vector& vector )
{
    for( int coordId = 0; coordId < m_dimension; coordId++ )
    {
        m_coordinates[ coordId ] = vector[ coordId ];
    }

    return *this;
}

Vector& Vector::operator+=( const Vector& vector )
{
    *this = *this + vector;

    return *this;
}

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