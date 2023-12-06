#ifndef PARTICLESPONTANEOUSCOLLISION_H
#define PARTICLESPONTANEOUSCOLLISION_H

#include "ParticleContactGenerator.h"

/**
 * @brief Détecte les collisions spontanées entre des particules (= pas de comportement particulier comme un générateur)
*/
class SpontaneousCollision : public ParticleContactGenerator
{
public:
    std::shared_ptr<Rigidbody> m_rigidbodies[2];

    // Coefficient de restitution du choc (rebond)
    float m_restitution;
public:
    virtual int addContact( Contact* contact, const int& limit ) const override;

    inline float penetrationOnAxis(const Vector3& axis, const Vector3& toCenter) const;
    bool testInterpenetration(int& bestCase, float& bestOverlap, const Vector3& toCenter) const;
    std::vector<Vector3> computeAxis() const;
    Vector3 contactPoint(const Vector3& pOne, const Vector3& dOne, float oneSize, const Vector3& pTwo, const Vector3& dTwo, float twoSize) const;
    void faceAxisContact(Contact* contact, std::shared_ptr<Rigidbody> one, std::shared_ptr<Rigidbody> two, const Vector3& toCenter, const int& best, const float& penetration) const;
    void edgeToEdgeContact(Contact* contact, const Vector3& toCenter, int best, const float& penetration) const;
};

#endif