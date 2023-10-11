#ifndef MATRIX_H
#define MATRIX_H

#define MATRIX_3_SIZE 9
#define MATRIX_4_SIZE 12

#include "Vector.h"
#include <array>

class Matrix3
{
private:
    // Une matrice = un tableau 1 dimension
    std::array<float, MATRIX_3_SIZE> m_matrix;

public:
    Matrix3( const std::array<float, MATRIX_3_SIZE>& matrixArray = {} );
    Matrix3( const Matrix3& matrix );

    // Setters
    void setMatrix( const std::array<float, MATRIX_3_SIZE>& matrix ) { m_matrix = matrix; }
    void setMatrix( const Matrix3& matrix ) { m_matrix = matrix.m_matrix; }

    const float& operator[]( const int& vectorIndex ) const { return m_matrix[ vectorIndex ]; }
    float& operator[]( const int& vectorIndex ) { return m_matrix[ vectorIndex ]; }

    // Opérations
    Matrix3 operator*( const float& value ) const;
    Vector3 operator*( const Vector3& vector ) const;
    Vector3 transform( const Vector3& vector ) const;

    Matrix3 operator*( const Matrix3& other ) const;
    Matrix3& operator*=( const Matrix3& other );

    Matrix3& operator=( const Matrix3& other );

    void setIdentity();
    void clear();

    void show( std::ostream& out = std::cout ) const;
};

#endif