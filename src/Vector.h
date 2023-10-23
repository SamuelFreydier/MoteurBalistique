#ifndef VECTOR_H
#define VECTOR_H

#include <ofMain.h>

// Un vecteur de dimension 3 => (x, y, z)
class Vector3
{
public:
    float x;
    float y;
    float z;
public:
    Vector3( const float& newX = 0, const float& newY = 0, const float& newZ = 0 );
    Vector3( const Vector3& vector );

    // Getters (pour s'assurer dans certains cas de retourner une référence constante)
    const float& getX() const { return x; }
    const float& getY() const { return y; }
    const float& getZ() const { return z; }

    // Vecteurs graphiques
    glm::vec1 v1() const { return glm::vec1( x ); }
    glm::vec2 v2() const { return glm::vec2( x, y ); }
    glm::vec3 v3() const { return glm::vec3( x, y, z ); }

    // Multiplication par un scalaire
    Vector3 operator*( const float& value ) const;
    Vector3& operator*=( const float& value );

    // Division par un scalaire
    Vector3 operator/( const float& value ) const;
    Vector3& operator/=( const float& value );

    // Produit vectoriel
    Vector3 operator*( const Vector3& vector ) const;

    // Produit scalaire
    float dotProduct( const Vector3& vector ) const;

    // surcharges d'operateurs
    Vector3 operator+( const Vector3& vector ) const;
    Vector3& operator+=(const Vector3& vector);

    Vector3 operator-() const;
    Vector3 operator-( const Vector3& vector ) const;
    Vector3& operator-=(const Vector3& vector);

    Vector3& operator=( const Vector3& vector );
    bool operator==( const Vector3& vector ) const;
    bool operator!=( const Vector3& vector ) const;

    // Nettoyage
    void clear();

    float norm() const;
    Vector3 normalized() const;
    void normalize();
    float distance( const Vector3& vector ) const;

    void show( std::ostream& out = std::cout ) const;
};

#endif