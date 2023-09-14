#include "Matrix.h"

Matrix::Matrix( const size_t& nbRows, const size_t& nbCols )
    : m_rows( nbRows ), m_cols( nbCols )
{
    m_matrix.reserve( m_cols );

    for( int cptCol = 0; cptCol < m_cols; cptCol++ ) 
    {
        m_matrix.emplace_back( Vector( m_rows ) );
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