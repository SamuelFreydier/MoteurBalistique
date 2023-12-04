#include "ParticleSpontaneousCollision.h"

/**
 * @brief Génère la collision lorsque deux particules rentrent en contact
 * @param contact 
 * @param limit 
 * @return 0 si pas de collision. 1 sinon.
*/
int ParticleSpontaneousCollision::addContact( ParticleContact* contact, const int& limit ) const
{
    // Particules
    //contact->m_particles[ 0 ] = m_particles[ 0 ];
    //contact->m_particles[ 1 ] = m_particles[ 1 ];
    //
    //// Vecteur entre les deux particules
    //Vector3 midline = m_particles[ 0 ]->getPosition() - m_particles[ 1 ]->getPosition();
    //float midlineSize = midline.norm();
    //
    //// Y a-t-il collision ?
    //if( midlineSize <= 0.0f || midlineSize >= m_particles[ 0 ]->getRadius() + m_particles[ 1 ]->getRadius() )
    //{
    //    // Non => distance plus grande que la somme des deux radius
    //    return 0;
    //}
    //
    //// Calcul du vecteur normal
    //contact->m_contactNormal = -midline.normalized();
    //
    //contact->m_penetration = m_particles[ 0 ]->getRadius() + m_particles[ 1 ]->getRadius() - midlineSize;
    //contact->m_restitution = m_restitution;
    //
    //return 1;

    // Rigidbodies
    int bestAxis = -1;
    float penetration = FLT_MAX;
    if (testInterpenetration(bestAxis, penetration)) return 0;

    // Find the vector between the two centres
    Vector3 toCenter = m_rigidbodies[0]->getAxis(3) - m_rigidbodies[0]->getAxis(3);

    if (bestAxis < 3)
    {
        faceAxisContact(contact, toCenter, bestAxis, penetration);
        return 1;
    }
    else if (bestAxis < 6)
    {
        faceAxisContact(contact, toCenter * -1.0f, bestAxis - 3, penetration);
        return 1;
    }
    else
    {
        edgeToEdgeContact(contact, toCenter, bestAxis, penetration);
        return 1;
    }
}

/**
 * @brief Calcule l'interpénétration des deux rigidbodies sur un axe de séparation
 * @param axis
 * @return Valeur positive si interpénétration, négative sinon
*/
inline float ParticleSpontaneousCollision::penetrationOnAxis(const Vector3& axis) const
{
    Vector3 toCenter = m_rigidbodies[0]->getPosition().distance(m_rigidbodies[1]->getPosition());

    float oneProject = m_rigidbodies[0]->transformToAxis(axis);
    float twoProject = m_rigidbodies[1]->transformToAxis(axis);

    float distance = fabs(toCenter.dotProduct(axis));

    return oneProject + twoProject - distance;
}

/**
 * @brief Calcule l'interpénétration des deux rigidbodies sur un axe de séparation
 * @param bestCase output (positif si interpénétration, négatif sinon)
 * @param bestOverlap output
*/
bool ParticleSpontaneousCollision::testInterpenetration(int& bestCase, float& bestOverlap) const
{
    float overlap;

    Vector3 currentAxis;
    std::vector<Vector3> axis = computeAxis();

    for (int i = 0; i < 15; ++i)
    {
        currentAxis = axis[i];

        if (currentAxis.squareMagnitude() < 0.001) continue;

        currentAxis.normalize();
        overlap = penetrationOnAxis(currentAxis);

        if (overlap < 0)
        {
            return false;
        }

        if (overlap < bestOverlap)
        {
            bestOverlap = overlap;
            bestCase = i;
        }
    }

    return true;
}

/**
 * @brief Calcule les 15 axes nécessaires au separating axis test
*/
std::vector<Vector3> ParticleSpontaneousCollision::computeAxis() const
{
    std::vector<Vector3> axis;

    // Axes des faces du rigidbody 1
    axis.push_back(m_rigidbodies[0]->getAxis(0));
    axis.push_back(m_rigidbodies[0]->getAxis(1));
    axis.push_back(m_rigidbodies[0]->getAxis(2));

    // Axes des faces du rigidbody 2
    axis.push_back(m_rigidbodies[1]->getAxis(0));
    axis.push_back(m_rigidbodies[1]->getAxis(1));
    axis.push_back(m_rigidbodies[1]->getAxis(2));

    // Axes des arrêtes des rigidbodies
    axis.push_back(axis[0] * axis[3]);
    axis.push_back(axis[0] * axis[4]);
    axis.push_back(axis[0] * axis[5]);
    axis.push_back(axis[1] * axis[3]);
    axis.push_back(axis[1] * axis[4]);
    axis.push_back(axis[1] * axis[5]);
    axis.push_back(axis[2] * axis[3]);
    axis.push_back(axis[2] * axis[4]);
    axis.push_back(axis[2] * axis[5]);

    return axis;
}

Vector3 ParticleSpontaneousCollision::contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize, bool useOne) const
{
    Vector3 toSt, cOne, cTwo;
    float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    float denom, mua, mub;

    smOne = dOne.squareMagnitude();
    smTwo = dTwo.squareMagnitude();
    dpOneTwo = dTwo.dotProduct(dOne);

    toSt = pOne - pTwo;
    dpStaOne = dOne.dotProduct(toSt);
    dpStaTwo = dTwo.dotProduct(toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    // Lignes parallèles
    if (fabs(denom) < 0.0001f) {
        return useOne ? pOne : pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    if (mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
    {
        return useOne ? pOne : pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}

/**
 * @brief Calcule les données de collision pour une collision Sommet - Face
 * @param Contact
 * @param toCenter : le vecteur entre les centres des deux boites
 * @param best : l'axe le plus optimisé pour le contact
 * @param penetration
*/
void ParticleSpontaneousCollision::faceAxisContact(ParticleContact* contact, const Vector3& toCenter, const int& best, const float& penetration) const
{
    // Face impliquée dans la collision
    Vector3 normal = m_rigidbodies[0]->getAxis(best);
    if (m_rigidbodies[0]->getAxis(best).dotProduct(toCenter) > 0)
    {
        normal = normal * -1.0f;
    }

    // Sommet impliqué dans la collision
    Vector3 vertex = m_rigidbodies[1]->halfsize();
    if (m_rigidbodies[1]->getAxis(0).dotProduct(normal) < 0) vertex.x = -vertex.x;
    if (m_rigidbodies[1]->getAxis(1).dotProduct(normal) < 0) vertex.y = -vertex.y;
    if (m_rigidbodies[1]->getAxis(2).dotProduct(normal) < 0) vertex.z = -vertex.z;

    contact->m_contactNormal = normal;
    contact->m_penetration = penetration;
    contact->m_contactPoint = m_rigidbodies[1]->getPointInLocalSpace(vertex);
    contact->m_restitution = m_restitution;
}


void ParticleSpontaneousCollision::edgeToEdgeContact(ParticleContact* contact, const Vector3& toCenter, int best, const float& penetration) const
{
    float bestSingleAxis = best;

    best -= 6;
    unsigned oneAxisIndex = best / 3;
    unsigned twoAxisIndex = best % 3;
    Vector3 oneAxis = m_rigidbodies[0]->getAxis(oneAxisIndex);
    Vector3 twoAxis = m_rigidbodies[1]->getAxis(twoAxisIndex);
    Vector3 axis = oneAxis * twoAxis;
    axis.normalize();

    if (axis.dotProduct(toCenter) > 0) axis = axis * -1.0f;

    // Points de contact sur les arêtes
    Vector3 ptOnOneEdge = m_rigidbodies[0]->halfsize();
    Vector3 ptOnTwoEdge = m_rigidbodies[1]->halfsize();

    for (int i = 0; i < 3; i++)
    {
        if (i == oneAxisIndex)
        {
            ptOnOneEdge.setCoordinate(i, 0);
        }
        else if (m_rigidbodies[0]->getAxis(i).dotProduct(axis) > 0)
        {
            ptOnOneEdge.setCoordinate(i, -ptOnOneEdge.getCoordinate(i));
        }

        if (i == twoAxisIndex)
        {
            ptOnTwoEdge.setCoordinate(i, 0);
        }
        else if (m_rigidbodies[1]->getAxis(i).dotProduct(axis) < 0)
        {
            ptOnTwoEdge.setCoordinate(i, -ptOnTwoEdge.getCoordinate(i));
        }
    }

    ptOnOneEdge = m_rigidbodies[0]->getPointInWorldSpace(ptOnOneEdge);
    ptOnTwoEdge = m_rigidbodies[1]->getPointInWorldSpace(ptOnTwoEdge);

    Vector3 vertex = contactPoint(
        ptOnOneEdge, oneAxis, m_rigidbodies[0]->halfsize().getCoordinate(oneAxisIndex),
        ptOnTwoEdge, twoAxis, m_rigidbodies[1]->halfsize().getCoordinate(twoAxisIndex),
        bestSingleAxis > 2
    );

    contact->m_contactNormal = axis;
    contact->m_penetration = penetration;
    contact->m_contactPoint = vertex;
    contact->m_restitution = m_restitution;
}
