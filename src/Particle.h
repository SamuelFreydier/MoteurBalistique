#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

class Particle {
private:
	float m_massReverse;
	float m_velocity;
	float m_acceleration;
	Vector m_position;

public:
	Particle(float mass, float velocity, float acceleration, Vector position);

};

#endif
