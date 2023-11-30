#include "RigidbodyCuboid.h"
#include <iostream>

RigidbodyCuboid::RigidbodyCuboid(const float width, const float depth, const float height, const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& angularVelocity, const Vector3& color)
    : Rigidbody(mass, velocity, position, angularVelocity, color), m_width(width), m_depth(depth), m_height(height)
{
    if (m_inverseMass != 0)
    {
        float coef = 1.0f/(12.0f * m_inverseMass);
        float coefx = (pow(m_depth, 2) + pow(m_height, 2)) *coef ;
        float coefy = (pow(m_width, 2) + pow(m_height, 2)) *coef;
        float coefz = (pow(m_depth, 2) + pow(m_width, 2)) *coef;
        setInertiaTensor(Matrix3x3(coefx, 0, 0, 0, coefy, 0, 0, 0, coefz));
    }
    else
        std::cout << "RigidBodyCuboid initialisation: m_inverseMass = 0" << std::endl;

    m_sphereCollider.m_radius = std::max( { m_width, m_height, m_depth } );
}

void RigidbodyCuboid::draw() const
{
    ofBoxPrimitive graphicCube;
    graphicCube.setPosition(m_position.v3());
    graphicCube.setWidth(m_width);
    graphicCube.setHeight(m_depth);
    graphicCube.setDepth(m_height);
    graphicCube.setOrientation(m_orientation.quat());

    ofSetColor(ofColor(0, 0, 0));
    graphicCube.drawWireframe();

    ofColor color(m_color.x, m_color.y, m_color.z);
    ofSetColor(color);

    graphicCube.draw();
}
