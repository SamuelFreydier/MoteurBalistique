#include "RigidbodyCube.h"

RigidbodyCube::RigidbodyCube(const float size, const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& angularVelocity, const Vector3& color)
    : Rigidbody(mass, velocity, position, angularVelocity, color), m_size(size)
{
}

void RigidbodyCube::draw() const
{
    ofBoxPrimitive graphicCube;
    graphicCube.setPosition(m_position.v3());
    graphicCube.setWidth(m_size);
    graphicCube.setHeight(m_size);
    graphicCube.setDepth(m_size);
    graphicCube.setOrientation(m_orientation.quat());

    ofColor color(m_color.x, m_color.y, m_color.z);
    ofSetColor(color);

    graphicCube.draw();
}
