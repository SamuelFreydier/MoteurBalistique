#ifndef VECTOR_H
#define VECTOR_H

#define DEFAULT_VCT_DIMENSION 3

#include <ofMain.h>

class Vector
{
private:
    std::vector<float> m_coordinates;
    size_t m_dimension;
public:
    Vector( const std::vector<float>& coordinates );
    Vector( const size_t& dimension = DEFAULT_VCT_DIMENSION );

    const size_t& getDimension() const { return m_dimension; }
    const std::vector<float>& getCoordinates() const { return m_coordinates; }
    const float& operator[]( const size_t& coordIndex ) const { return m_coordinates[ coordIndex ]; }
    float& operator[]( const size_t& coordIndex ) { return m_coordinates[ coordIndex ]; }

    // getters
    const float& getX() const { return m_coordinates[ 0 ]; }
    const float& getY() const { return m_coordinates[ 1 ]; }
    const float& getZ() const { return m_coordinates[ 2 ]; }
    const float& getW() const { return m_coordinates[ 3 ]; }


    // /!Attention!\ Peut planter si dimension trop basse
    glm::vec1 v1() const { return glm::vec1( m_coordinates[ 0 ] ); }
    glm::vec2 v2() const { return glm::vec2( m_coordinates[ 0 ], m_coordinates[ 1 ] ); }
    glm::vec3 v3() const { return glm::vec3( m_coordinates[ 0 ], m_coordinates[ 1 ], m_coordinates[ 2 ] ); }
    glm::vec4 v4() const { return glm::vec4( m_coordinates[ 0 ], m_coordinates[ 1 ], m_coordinates[ 2 ], m_coordinates[ 3 ] ); }

    // Multiplication par un scalaire
    Vector operator*( const float& value ) const;
    Vector& operator*=( const float& value );

    // Produit vectoriel
    Vector operator*( const Vector& vector ) const;

    // Produit scalaire
    float dotProduct( const Vector& vector ) const;

    // surcharges d'operateurs
    Vector operator+( const Vector& vector ) const;
    Vector& operator=( const Vector& vector );
    Vector& operator+=( const Vector& vector );
    bool operator==( const Vector& vector ) const;
    bool operator!=( const Vector& vector ) const;

    float norm() const;
    Vector normalized() const;
    void normalize();

    void show( std::ostream& out = std::cout ) const;
};

#endif