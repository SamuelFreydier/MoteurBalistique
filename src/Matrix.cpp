#include "Matrix.h"

Matrix3::Matrix3( const std::array<float, MATRIX_3_SIZE>& matrixArray )
    : m_matrix( matrixArray )
{
}

Matrix3::Matrix3( const Matrix3& matrix )
    : Matrix3( matrix.m_matrix )
{
}

/**
 * @brief Affectation à la matrice identité
*/
void Matrix3::setIdentity()
{
    clear();

    m_matrix[ 0 ] = m_matrix[ 4 ] = m_matrix[ 8 ] = 1;
}

/**
 * @brief Affectation à la matrice nulle
*/
void Matrix3::clear()
{
    m_matrix = {};
}


/**
 * @brief Multiplication par un scalaire
 * @param value 
 * @return 
*/
Matrix3 Matrix3::operator*( const float& value ) const
{
    Matrix3 newMatrix( *this );

    for( float& matrixElement : newMatrix.m_matrix )
    {
        matrixElement *= value;
    }

    return newMatrix;
}

/**
 * @brief Transforme le vecteur par la matrice courante
 * @param vector 
 * @return 
*/
Vector3 Matrix3::operator*( const Vector3& vector ) const
{
    return Vector3(
        vector.getX() * m_matrix[ 0 ] + vector.getY() * m_matrix[ 1 ] + vector.getZ() * m_matrix[ 2 ],
        vector.getX() * m_matrix[ 3 ] + vector.getY() * m_matrix[ 4 ] + vector.getZ() * m_matrix[ 5 ],
        vector.getX() * m_matrix[ 6 ] + vector.getY() * m_matrix[ 7 ] + vector.getZ() * m_matrix[ 8 ]
    );
}

Vector3 Matrix3::transform( const Vector3& vector ) const
{
    return ( *this ) * vector;
}

/**
 * @brief Produit matriciel. Hardcodé car il n'est jamais prévu de le changer => Quelques gains minimes de performance.
 * @param matrix
 * @return
*/
Matrix3 Matrix3::operator*( const Matrix3& other ) const
{
    return Matrix3( {
        m_matrix[ 0 ] * other.m_matrix[ 0 ] + m_matrix[ 1 ] * other.m_matrix[ 3 ] + m_matrix[ 2 ] * other.m_matrix[ 6 ],
        m_matrix[ 0 ] * other.m_matrix[ 1 ] + m_matrix[ 1 ] * other.m_matrix[ 4 ] + m_matrix[ 2 ] * other.m_matrix[ 7 ],
        m_matrix[ 0 ] * other.m_matrix[ 2 ] + m_matrix[ 1 ] * other.m_matrix[ 5 ] + m_matrix[ 2 ] * other.m_matrix[ 8 ],

        m_matrix[ 3 ] * other.m_matrix[ 0 ] + m_matrix[ 4 ] * other.m_matrix[ 3 ] + m_matrix[ 5 ] * other.m_matrix[ 6 ],
        m_matrix[ 3 ] * other.m_matrix[ 1 ] + m_matrix[ 4 ] * other.m_matrix[ 4 ] + m_matrix[ 5 ] * other.m_matrix[ 7 ],
        m_matrix[ 3 ] * other.m_matrix[ 2 ] + m_matrix[ 4 ] * other.m_matrix[ 5 ] + m_matrix[ 5 ] * other.m_matrix[ 8 ],

        m_matrix[ 6 ] * other.m_matrix[ 0 ] + m_matrix[ 7 ] * other.m_matrix[ 3 ] + m_matrix[ 8 ] * other.m_matrix[ 6 ],
        m_matrix[ 6 ] * other.m_matrix[ 1 ] + m_matrix[ 7 ] * other.m_matrix[ 4 ] + m_matrix[ 8 ] * other.m_matrix[ 7 ],
        m_matrix[ 6 ] * other.m_matrix[ 2 ] + m_matrix[ 7 ] * other.m_matrix[ 5 ] + m_matrix[ 8 ] * other.m_matrix[ 8 ]
                    } );
}


/**
 * @brief Produit matriciel.
 * @param other 
 * @return 
*/
Matrix3& Matrix3::operator*=( const Matrix3& other )
{
    *this = *this * other;
    return *this;
}


/**
 * @brief Affectation à une Matrice à 3 dimensions
 * @param other 
 * @return 
*/
Matrix3& Matrix3::operator=( const Matrix3& other )
{
    for( int matrixIndex = 0; matrixIndex < MATRIX_3_SIZE; matrixIndex++ )
    {
        m_matrix[ matrixIndex ] = other[ matrixIndex ];
    }

    return *this;
}


/**
 * @brief Affichage de la matrice
 * @param out 
*/
void Matrix3::show( std::ostream& out ) const
{
    for( int cptRow = 0; cptRow < 3; cptRow++ )
    {
        for( int cptCol = 0; cptCol < 3; cptCol++ )
        {
            out << m_matrix[ cptRow * 3 + cptCol ] << " ";
        }
        out << std::endl;
    }

    out << std::endl;
}