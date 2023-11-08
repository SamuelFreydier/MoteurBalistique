#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleSpring.h"

// Define a tolerance for floating-point comparisons
const float EPSILON = 1e-6;

TEST(ParticleSpringTest, UpdateForce) {
    float duration = 1.0f;
    std::shared_ptr<Particle> particle1 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 0.0f, 0.0f));
    std::shared_ptr<Particle> particle2 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(0.0f, 0.0f, 0.0f));
    ParticleSpring springForce = ParticleSpring(particle2, 1.0f, 2.0f);

    springForce.updateForce(particle1, duration);

    Vector3 force = particle1->getAccumForce();

    // Expected force calculation
    Vector3 springVector = particle1->getPosition() - particle2->getPosition();
    float length = springVector.norm();
    float expectedNorm = -springForce.getSpringConstant() * (length - springForce.getRestLength());
    Vector3 expectedForce = springVector.normalized() * expectedNorm;

    ASSERT_NEAR(force.x, expectedForce.x, EPSILON);
    ASSERT_NEAR(force.y, expectedForce.y, EPSILON);
    ASSERT_NEAR(force.z, expectedForce.z, EPSILON);
}