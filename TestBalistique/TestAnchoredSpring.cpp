#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleAnchoredSpring.h"

namespace AnchoredSpringTest
{
    const float EPSILON = 1e-6;

    TEST(TestAnchoredSpring, UpdateForce) {
        float duration = 1.0f;

        Vector3 anchor = Vector3(0.0f, 0.0f, 0.0f);
        ParticleAnchoredSpring anchoredSpring = ParticleAnchoredSpring(&anchor, 1.0f, 2.0f);
        std::shared_ptr<Particle> particle = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 0.0f, 0.0f));

        anchoredSpring.updateForce(particle, duration);

        Vector3 force = particle->getAccumForce();

        Vector3 springVector = particle->getPosition() - anchor;
        float length = springVector.norm();
        float expectedNorm = -anchoredSpring.getSpringConstant() * (length - anchoredSpring.getRestLength());
        Vector3 expectedForce = springVector.normalized() * expectedNorm;

        ASSERT_NEAR(force.x, expectedForce.x, EPSILON);
        ASSERT_NEAR(force.y, expectedForce.y, EPSILON);
        ASSERT_NEAR(force.z, expectedForce.z, EPSILON);
    }
}