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


        m_localVertices.push_back(Vector3(m_width, m_height, m_depth) / 2.0);
        m_localVertices.push_back(Vector3(m_width, m_height, -m_depth) / 2.0);
        m_localVertices.push_back(Vector3(m_width, -m_height, m_depth) / 2.0);
        m_localVertices.push_back(Vector3(-m_width, m_height, m_depth) / 2.0);
        m_localVertices.push_back(Vector3(m_width, -m_height, -m_depth) / 2.0);
        m_localVertices.push_back(Vector3(-m_width, -m_height, m_depth) / 2.0);
        m_localVertices.push_back(Vector3(-m_width, m_height, -m_depth) / 2.0);
        m_localVertices.push_back(Vector3(-m_width, -m_height, -m_depth) / 2.0);
    }
    else
        std::cout << "RigidBodyCuboid initialisation: m_inverseMass = 0" << std::endl;
}

std::vector<Vector3> RigidbodyCuboid::getVerticesGlobalPosition() const
{
    std::vector<Vector3> vertices;

    for (Vector3 vertex : m_localVertices)
    {
        vertices.push_back(getPointInWorldSpace(vertex));
    }

    return vertices;
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

float RigidbodyCuboid::transformToAxis(const Vector3& axis) const
{
    float value = halfsize().x * fabs(axis.dotProduct(getAxis(0)));
          value += halfsize().y * fabs(axis.dotProduct(getAxis(1)));
          value += halfsize().z * fabs(axis.dotProduct(getAxis(2)));

    return value;
}

Vector3 RigidbodyCuboid::getAxis(int axis) const
{
    Vector3 computedAxis;

    if (axis >= 0 && axis < 3)
    {
        std::vector<Vector3> vertices = getVerticesGlobalPosition();

        switch (axis)
        {
            case 0:
                computedAxis = (vertices[2] - vertices[0]) * (vertices[1] - vertices[0]);
                break;
            case 1:
                computedAxis = (vertices[1] - vertices[0]) * (vertices[3] - vertices[0]);
                break;
            case 2:
                computedAxis = (vertices[3] - vertices[0]) * (vertices[2] - vertices[0]);
                break;
        }
    }

    return computedAxis;
}

Vector3 RigidbodyCuboid::halfsize() const
{
    return Vector3(m_width / 2, m_height / 2, m_depth / 2);
}
