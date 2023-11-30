#include "RigidbodyCube.h"

RigidbodyCube::RigidbodyCube(const float size, const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& angularVelocity, const Vector3& color)
    : Rigidbody(mass, velocity, position, angularVelocity, color), m_size(size)
{
    float coeff = (6.0f * m_inverseMass / pow(m_size, 2));
    m_inverseInertiaTensor = Matrix3x3(coeff, 0, 0, 0, coeff, 0, 0, 0, coeff);
    m_sphereCollider.m_radius = m_size;
}

void RigidbodyCube::draw() const
{
    ofBoxPrimitive graphicCube;
    graphicCube.setPosition(m_position.v3());
    graphicCube.setWidth(m_size);
    graphicCube.setHeight(m_size);
    graphicCube.setDepth(m_size);
    graphicCube.setOrientation(m_orientation.quat());

    ofSetColor(ofColor(0, 0, 0));
    graphicCube.drawWireframe();

    ofColor color(m_color.x, m_color.y, m_color.z);
    ofSetColor(color);

    graphicCube.draw();
}
