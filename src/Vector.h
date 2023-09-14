#ifndef VECTOR_H
#define VECTOR_H

#include <ofMain.h>

class Vector
{
    private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
    public:
    Vector( const float& newX = 0, const float& newY = 0, const float& newZ = 0, const float& newW = 0 );

    const float& getX() const { return m_x; };
    const float& getY() const { return m_y; };
    const float& getZ() const { return m_z; };
    const float& getW() const { return m_w; };

    glm::vec1 v1() const { return glm::vec1( m_x ); }
    glm::vec2 v2() const { return glm::vec2( m_x, m_y ); };
    glm::vec3 v3() const { return glm::vec3( m_x, m_y, m_z ); };
    glm::vec4 v4() const { return glm::vec4( m_x, m_y, m_z, m_w ); }

    // Multiplication par un scalaire
    Vector operator*( const float& value ) const;

    // Produit vectoriel
    Vector operator*( const Vector& vector ) const;

    // Produit scalaire
    float dot( const Vector& vector ) const;

    Vector operator+( const Vector& vector ) const;
    Vector& operator=( const Vector& vector );
    Vector& operator+=( const Vector& vector );
    bool operator==( const Vector& vector ) const;
    bool operator!=( const Vector& vector ) const;

    float norm() const;
    void normalize();
};

#endif