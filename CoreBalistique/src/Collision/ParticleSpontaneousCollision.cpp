#include "ParticleSpontaneousCollision.h"

/**
 * @brief Génère la collision lorsque deux particules rentrent en contact
 * @param contact 
 * @param limit 
 * @return 0 si pas de collision. 1 sinon.
*/
int SpontaneousCollision::addContact( Contact* contact, const int& limit ) const
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

    // Vecteur entre les deux centres
    Vector3 toCenter = m_rigidbodies[1]->getPosition() - m_rigidbodies[0]->getPosition();

    if (!testInterpenetration(bestAxis, penetration, toCenter)) return 0;


    // Sommet du body 1 sur une face du body 2
    if (bestAxis < 3)
    {
        faceAxisContact(contact, m_rigidbodies[0], m_rigidbodies[1], toCenter, bestAxis, penetration);
        return 1;
    }
    // Sommet du body 2 sur une face du body 1
    else if (bestAxis < 6)
    {
        faceAxisContact(contact, m_rigidbodies[1], m_rigidbodies[0], toCenter * -1.0f, bestAxis - 3, penetration);
        return 1;
    }
    // Contact d'arêtes
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
inline float SpontaneousCollision::penetrationOnAxis(const Vector3& axis, const Vector3& toCenter) const
{
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
bool SpontaneousCollision::testInterpenetration(int& bestCase, float& bestOverlap, const Vector3& toCenter) const
{
    float overlap;

    Vector3 currentAxis;
    
    std::vector<Vector3> axis = computeAxis();

    for (int i = 0; i < 15; ++i)
    {
        currentAxis = axis[i];

        if (currentAxis.squareMagnitude() < 0.001) continue;

        currentAxis.normalize();
        overlap = penetrationOnAxis(currentAxis, toCenter);

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
std::vector<Vector3> SpontaneousCollision::computeAxis() const
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

/**
 * @brief Point de contact en cas de collision arête - arête
 * @param pOne : point de contact sur premier body
 * @param dOne : axe considéré pour le premier body
 * @param oneSize : taille de l'arête considérée pour le premier body
 * @param pTwo : point de contact sur deuxième body
 * @param dTwo : axe considéré pour le deuxième body
 * @param twoSize : taille de l'arête considérée pour le premier body
*/
Vector3 SpontaneousCollision::contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize) const
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

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    cOne = pOne + dOne * mua;
    cTwo = pTwo + dTwo * mub;

    return cOne * 0.5 + cTwo * 0.5;
}

/**
 * @brief Calcule les données de collision pour une collision Sommet - Face
 * @param Contact
 * @param toCenter : le vecteur entre les centres des deux boites
 * @param best : l'axe le plus important pour le contact
 * @param penetration
*/
void SpontaneousCollision::faceAxisContact(Contact* contact, std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two, const Vector3& toCenter, const int& best, const float& penetration) const
{
    // Face impliquée dans la collision
    Vector3 normal = one->getAxis(best);
    if (one->getAxis(best).dotProduct(toCenter) < 0)
    {
        normal = normal * -1.0f;
    }

    // Sommet impliqué dans la collision
    Vector3 vertex = two->halfsize();
    if (two->getAxis(0).dotProduct(normal) > 0) vertex.x = -vertex.x;
    if (two->getAxis(1).dotProduct(normal) > 0) vertex.y = -vertex.y;
    if (two->getAxis(2).dotProduct(normal) > 0) vertex.z = -vertex.z;

    contact->m_contactNormal = normal;
    contact->m_penetration = penetration;
    contact->m_contactPoint = two->getPointInWorldSpace(vertex);
    contact->m_restitution = m_restitution;
}

/**
 * @brief Calcule les données de collision pour une collision Arête - arête
 * @param Contact
 * @param toCenter : le vecteur entre les centres des deux boites
 * @param best : l'axe le plus important pour le contact
 * @param penetration
*/
void SpontaneousCollision::edgeToEdgeContact(Contact* contact, const Vector3& toCenter, int best, const float& penetration) const
{
    float bestSingleAxis = best;

    best -= 6;
    unsigned oneAxisIndex = best / 3;
    unsigned twoAxisIndex = best % 3;
    Vector3 oneAxis = m_rigidbodies[0]->getAxis(oneAxisIndex);
    Vector3 twoAxis = m_rigidbodies[1]->getAxis(twoAxisIndex);
    Vector3 axis = oneAxis * twoAxis;
    axis.normalize();

    if (axis.dotProduct(toCenter) < 0) axis = axis * -1.0f;

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
        ptOnTwoEdge, twoAxis, m_rigidbodies[1]->halfsize().getCoordinate(twoAxisIndex)
    );

    contact->m_contactNormal = axis;
    contact->m_penetration = penetration;
    contact->m_contactPoint = vertex;
    contact->m_restitution = m_restitution;
}
