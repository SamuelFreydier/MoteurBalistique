#include "Matrix.h"

Matrix::Matrix( const int& nbRows, const int& nbCols )
    : m_rows( nbRows ), m_cols( nbCols )
{
    m_matrix.reserve( m_cols );

    for( int cptCol = 0; cptCol < m_cols; cptCol++ )
    {
        m_matrix.emplace_back( Vector( m_rows ) );
    }
}

Matrix::Matrix( const Matrix& matrix )
    : Matrix( matrix.m_rows, matrix.m_cols )
{
    for( int cptRow = 0; cptRow < m_rows; cptRow++ )
    {
        for( int cptCol = 0; cptCol < m_cols; cptCol++ )
        {
            m_matrix[ cptCol ][ cptRow ] = matrix[ cptCol ][ cptRow ];
        }
    }
}

void Matrix::setMatrix( float values[] )
{
    m_matrix.clear();
    m_matrix.reserve( m_cols );

    for( int cptCol = 0; cptCol < m_cols; cptCol++ )
    {
        m_matrix.emplace_back( Vector( m_rows ) );
    }

    for( int cptRow = 0; cptRow < m_rows; cptRow++ )
    {
        for( int cptCol = 0; cptCol < m_cols; cptCol++ )
        {
            m_matrix[ cptCol ][ cptRow ] = values[ cptRow * m_cols + cptCol ];
        }
    }
}

void Matrix::setIdentity()
{
    for( int cptRow = 0; cptRow < m_rows; cptRow++ )
    {
        for( int cptCol = 0; cptCol < m_cols; cptCol++ )
        {
            if( cptRow == cptCol )
            {
                m_matrix[ cptCol ][ cptRow ] = 1.0;
            }
            else
            {
                m_matrix[ cptCol ][ cptRow ] = 0.0;
            }
        }
    }
}

void Matrix::setZero()
{
    for( int cptRow = 0; cptRow < m_rows; cptRow++ )
    {
        for( int cptCol = 0; cptCol < m_cols; cptCol++ )
        {
            m_matrix[ cptCol ][ cptRow ] = 0.0;
        }
    }
}

Matrix Matrix::operator*( const float& value ) const
{
    Matrix newMatrix( *this );

    for( Vector& col : newMatrix.m_matrix )
    {
        col *= value;
    }

    return newMatrix;
}

/**
 * @brief Produit matriciel
 * @param matrix
 * @return
*/
Matrix Matrix::operator*( const Matrix& matrix ) const
{
    // TODO ? => Ici, on pourrait renvoyer une exception si les conditions de la multiplication matricielle ne sont pas validées
    // Pour le moment, on renvoie juste la matrice nulle
    Matrix newMatrix( m_rows, matrix.m_cols );

    // Verif nbCol de matrice A = nbLignes de matrice B
    if( m_cols == matrix.m_rows )
    {
        for( int cptRow = 0; cptRow < m_rows; cptRow++ )
        {
            for( int cptCol = 0; cptCol < matrix.m_cols; cptCol++ )
            {
                for( int cptCalcul = 0; cptCalcul < m_cols; cptCalcul++ )
                {
                    newMatrix[ cptCol ][ cptRow ] += m_matrix[ cptCalcul ][ cptRow ] * matrix[ cptCol ][ cptCalcul ];
                }
            }
        }
    }

    return newMatrix;
}

void Matrix::show( std::ostream& out ) const
{
    for( int cptRow = 0; cptRow < m_rows; cptRow++ )
    {
        for( int cptCol = 0; cptCol < m_cols; cptCol++ )
        {
            out << m_matrix[ cptCol ][ cptRow ] << " ";
        }
        out << std::endl;
    }

    out << std::endl;
}