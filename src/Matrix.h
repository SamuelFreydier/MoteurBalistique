#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <vector>

class Matrix
{
private:
    // On considère une matrice composée de vecteurs colonnes
    std::vector<Vector> m_matrix;

    int m_rows; // = m_dimension sur tous les Vector de la Matrix
    int m_cols;
public:
    Matrix( const int& nbRows, const int& nbCols );
    Matrix( const Matrix& matrix );

    // Getters
    const int& getNbRows() const { return m_rows; }
    const int& getNbColumns() const { return m_cols; }

    // Setters
    void setMatrix( float values[] );

    const Vector& operator[]( const int& vectorIndex ) const { return m_matrix[ vectorIndex ]; }
    Vector& operator[]( const int& vectorIndex ) { return m_matrix[ vectorIndex ]; }
    Matrix operator*( const float& value ) const;
    Matrix operator*( const Matrix& matrix ) const;

    void setIdentity();
    void setZero();

    void show( std::ostream& out = std::cout ) const;
};

#endif