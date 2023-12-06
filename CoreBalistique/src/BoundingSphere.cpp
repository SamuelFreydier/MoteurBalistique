#include "BoundingSphere.h"
/**
 * @brief  Constructeur d'une sph�re englobante
 * @param radius
 * @param position
*/
BoundingSphere::BoundingSphere(float radius, const Vector3& position)
    : m_radius(radius), m_position(position)
{

}

/**
 * @brief  D�tecte s'il y a collision entre la sph�re courante et une autre sph�res
 * @param other
 * @return true = collision d�tect�e
*/
bool BoundingSphere::collides(const BoundingSphere& other) const
{
    // C'est le travail de S�bastien :eyes:
    return (m_radius + other.getRadius() >= m_position.distance(other.getPosition()));
}
