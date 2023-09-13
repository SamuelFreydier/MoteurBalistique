#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
class Vector
{
private:
	float x, y, z, w;
public:
	float getX() { return x; };
	float getY() { return y; };
	float getZ() { return z; };
	float getW() { return w; };
	Vector(float newX, float newY, float newZ, float newW);

	glm::vec2 v2() { return glm::vec2(x, y); };
	glm::vec3 v3() { return glm::vec3(x, y, z); };
	
	Vector operator* (float multiply);
	Vector operator+ (Vector addVector);
	float norme();
	void normalisation();

};

