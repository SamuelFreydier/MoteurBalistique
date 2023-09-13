#include "Vector.h"
#include <math.h>

Vector::Vector(const float& newX, const float& newY, const float& newZ, const float& newW)
	: m_x(newX), m_y(newY), m_z(newZ), m_w(newW)
{
}

Vector Vector::operator*(const float& multiply) const
{
	return Vector(m_x * multiply, m_y * multiply, m_z * multiply, m_w * multiply);
}

Vector Vector::operator+(const Vector& addVector) const
{
	return Vector(m_x + addVector.getX(), m_y + addVector.getY(), m_z + addVector.getZ(), m_w + addVector.getW());
}

float Vector::norm() const
{
	return sqrt(m_x*m_x+m_y*m_y+m_z*m_z+m_w*m_w);
}

void Vector::normalize()
{
	float normeVar = norm();

	if (norm != 0) 
	{
		m_x /= normeVar;
		m_y /= normeVar;
		m_z /= normeVar;
		m_w /= normeVar;
	}
}

Vector& Vector::operator=(const Vector& vector)
{
	m_x = vector.m_x;
	m_y = vector.m_y;
	m_z = vector.m_z;
	m_w = vector.m_w;

	return *this;
}

Vector& Vector::operator+=(const Vector& vector)
{
	*this = *this + vector;

	return *this;
}

bool Vector::operator==(const Vector& vector) const
{
	return m_x == vector.m_x && m_y == vector.m_y && m_z == vector.m_z && m_w == vector.m_w;
}

bool Vector::operator!=(const Vector& vector) const
{
	return !(*this == vector);
}