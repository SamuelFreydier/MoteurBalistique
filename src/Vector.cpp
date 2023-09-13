#include "Vector.h"
#include <math.h>

Vector::Vector(const float& newX, const float& newY, const float& newZ, const float& newW)
	: x(newX), y(newY), z(newZ), w(newW)
{
}

Vector Vector::operator*(const float& multiply) const
{
	return Vector(x * multiply, y * multiply, z * multiply, w * multiply);
}

Vector Vector::operator+(const Vector& addVector) const
{
	return Vector(x + addVector.getX(), y + addVector.getY(), z + addVector.getZ(), w + addVector.getW());
}

float Vector::norme() const
{
	return sqrt(x*x+y*y+z*z+w*w);
}

void Vector::normalisation()
{
	float norme_ = norme();
	x /= norme_;
	y /= norme_;
	z /= norme_;
	w /= norme_;
}

Vector& Vector::operator=(const Vector& vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;

	return *this;
}

Vector& Vector::operator+=(const Vector& vector)
{
	*this = *this + vector;

	return *this;
}

bool Vector::operator==(const Vector& vector) const
{
	return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
}

bool Vector::operator!=(const Vector& vector) const
{
	return !(*this == vector);
}