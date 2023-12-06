#pragma once
#include "Vector.h"

// Classe de sphere englobante
class BoundingSphere
{
public:
    BoundingSphere(float radius = 1.f, const Vector3& position = Vector3());

    float m_radius;
    Vector3 m_position;

    bool collides(const BoundingSphere& other) const;
    const Vector3& getPosition() const { return m_position; }
    const float& getRadius() const { return m_radius; }
    void setPosition(const Vector3& position) { m_position = position; }
};

