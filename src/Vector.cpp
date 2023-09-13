#include "Vector.h"
#include <math.h>

Vector::Vector(float newX, float newY, float newZ, float newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

Vector Vector::operator*(float multiply)
{
	x *= multiply;
	y *= multiply;
	z *= multiply;
	w *= multiply;
	return *this;
}

Vector Vector::operator+(Vector addVector)
{
	x += addVector.getX();
	y += addVector.getY();
	z += addVector.getZ();
	w += addVector.getW();
	return *this;
}

float Vector::norme()
{
	return sqrt(x*x+y*y+z*z+w*w);
}

void Vector::normalisation()
{
	float norme_ = norme();
	x = x / norme_;
	y = y / norme_;
	z = z / norme_;
	w = w / norme_;
}
