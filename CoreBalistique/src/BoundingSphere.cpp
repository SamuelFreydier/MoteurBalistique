#include "BoundingSphere.h"
/**
 * @brief  Constructeur d'une sphère englobante
 * @param radius
 * @param position
*/
BoundingSphere::BoundingSphere(float radius, const Vector3& position)
    : m_radius(radius), m_position(position)
{

}

/**
 * @brief  Détecte s'il y a collision entre la sphère courante et une autre sphères
 * @param other
 * @return true = collision détectée
*/
bool BoundingSphere::collides(const BoundingSphere& other) const
{
    // C'est le travail de Sébastien :eyes:
    return (m_radius + other.getRadius() >= m_position.distance(other.getPosition()));
}
