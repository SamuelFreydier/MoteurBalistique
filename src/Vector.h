#ifndef VECTOR_H
#define VECTOR_H

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class Vector
{
private:
	float x, y, z, w;
public:
	Vector(const float& newX = 0, const float& newY = 0, const float& newZ = 0, const float& newW = 0);

	const float& getX() const { return x; };
	const float& getY() const { return y; };
	const float& getZ() const { return z; };
	const float& getW() const { return w; };

	glm::vec2 v2() { return glm::vec2(x, y); };
	glm::vec3 v3() { return glm::vec3(x, y, z); };
	
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