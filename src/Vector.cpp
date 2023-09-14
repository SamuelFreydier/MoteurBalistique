#include "Vector.h"
#include <math.h>

Vector::Vector( const float& newX, const float& newY, const float& newZ, const float& newW )
    : m_x( newX ), m_y( newY ), m_z( newZ ), m_w( newW )
{
}

Vector Vector::operator*( const float& value ) const
{
    return Vector( m_x * value, m_y * value, m_z * value, m_w * value );
}

/**
 * @brief Produit vectoriel (ignore w)
 * @param vector 
 * @return 
*/
Vector Vector::operator*( const Vector& vector ) const
{
    return Vector( m_y * vector.m_z - m_z * vector.m_y, m_z * vector.m_x - m_x * vector.m_z, m_x * vector.m_y - m_y * vector.m_x );
}

/**
 * @brief Produit scalaire
 * @param vector 
 * @return 
*/
float Vector::dot( const Vector& vector ) const
{
    return m_x * vector.m_x + m_y * vector.m_y + m_z * vector.m_z + m_w * vector.m_w;
}

Vector Vector::operator+( const Vector& vector ) const
{
    return Vector( m_x + vector.getX(), m_y + vector.getY(), m_z + vector.getZ(), m_w + vector.getW() );
}

float Vector::norm() const
{
    return sqrt( m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w );
}

void Vector::normalize()
{
    float vctNorm = norm();

    if( vctNorm != 0 )
    {
        m_x /= vctNorm;
        m_y /= vctNorm;
        m_z /= vctNorm;
        m_w /= vctNorm;
    }
}

Vector& Vector::operator=( const Vector& vector )
{
    m_x = vector.m_x;
    m_y = vector.m_y;
    m_z = vector.m_z;
    m_w = vector.m_w;

    return *this;
}

Vector& Vector::operator+=( const Vector& vector )
{
    *this = *this + vector;

    return *this;
}

bool Vector::operator==( const Vector& vector ) const
{
    return m_x == vector.m_x && m_y == vector.m_y && m_z == vector.m_z && m_w == vector.m_w;
}

bool Vector::operator!=( const Vector& vector ) const
{
    return !( *this == vector );
}