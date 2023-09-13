#ifndef VECTOR_H
#define VECTOR_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Vector
{
private:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
public:
	Vector(const float& newX = 0, const float& newY = 0, const float& newZ = 0, const float& newW = 0);

	const float& getX() const { return m_x; };
	const float& getY() const { return m_y; };
	const float& getZ() const { return m_z; };
	const float& getW() const { return m_w; };

	glm::vec2 v2() { return glm::vec2(m_x, m_y); };
	glm::vec3 v3() { return glm::vec3(m_x, m_y, m_z); };
	
	Vector operator*(const float& multiply) const;
	Vector operator+(const Vector& addVector) const;
	Vector& operator=(const Vector& vector);
	Vector& operator+=(const Vector& vector);
	bool operator==(const Vector& vector) const;
	bool operator!=(const Vector& vector) const;

	float norme() const;
	void normalisation();
};

#endif