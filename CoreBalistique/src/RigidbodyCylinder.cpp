#include "RigidbodyCylinder.h"

RigidbodyCylinder::RigidbodyCylinder(const float radius, const float height, const float& mass, const Vector3& velocity, const Vector3& position, const Vector3& angularVelocity, const Vector3& color)
	: Rigidbody(mass, velocity, position, angularVelocity, color), m_radius(radius), m_height(height)
{
    if (m_inverseMass != 0)
    {
        float coef = 1.0f / (12.0f * m_inverseMass);
        float coefxxyy = 3 * coef * pow(radius, 2) + coef * pow(height, 2);
        float coefz = 6*coef * pow(height, 2);
        /* 
        * | (m*r^2)/4 + (m*h^2)/2 0 0 |
        * |0 (m*r^2)/4 + (m*h^2)/2  0 |
        * | 0 0 (m*h^2)/2             |
        * Pour un cylindre PLEIN
        * 
        * 
        */
        setInertiaTensor(Matrix3x3(coefxxyy, 0, 0, 0, coefxxyy, 0, 0, 0, coefz)); 
    }
    else
        std::cout << "RigidBodyCylinder initialisation: m_inverseMass = 0" << std::endl;
}

void RigidbodyCylinder::draw() const
{
    ofCylinderPrimitive graphicCylinder;
    graphicCylinder.setPosition(m_position.v3());
    graphicCylinder.setRadius(m_radius);
    graphicCylinder.setHeight(m_height);
    graphicCylinder.setOrientation(m_orientation.quat());

    ofColor color(m_color.x, m_color.y, m_color.z);
    ofSetColor(color);

    graphicCylinder.draw();
}
