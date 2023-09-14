#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <vector>

class Matrix
{
private:
    // On considère une matrice composée de vecteurs colonnes
    std::vector<Vector> m_matrix;

    size_t m_rows; // = m_dimension sur tous les Vector de la Matrix
    size_t m_cols;
public:
    Matrix( const size_t& nbRows, const size_t& nbCols );
    Matrix( const Matrix& matrix );

    // Getters
    const size_t& getNbRows() const { return m_rows; }
    const size_t& getNbColumns() const { return m_cols; }

    // Setters
    void setMatrix( float values[] );

    const Vector& operator[]( const size_t& vectorIndex ) const { return m_matrix[ vectorIndex ]; }
    Vector& operator[]( const size_t& vectorIndex ) { return m_matrix[ vectorIndex ]; }
    Matrix operator*( const float& value ) const;
    Matrix operator*( const Matrix& matrix ) const;

    void setIdentity();
    void setZero();

    void show(std::ostream& out = std::cout) const;
};

#endif