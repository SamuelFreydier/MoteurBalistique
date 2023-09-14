#include "Particle.h"

Particle::Particle( float mass, float velocity, float acceleration, Vector position )
    : m_massReverse( 1 / mass ), m_velocity( velocity ), m_acceleration( acceleration ), m_position( position )
{
}
