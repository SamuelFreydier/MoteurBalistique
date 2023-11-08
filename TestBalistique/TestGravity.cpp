#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleGravity.h"

// Define a tolerance for floating-point comparisons
const float EPSILON = 1e-6;

// Test fixture class for ParticleGravity
class ParticleGravityTest : public testing::Test {
protected:
    ParticleGravity gravityForce;
    std::shared_ptr<Particle> particle;
    Vector3 gravity;

    void SetUp() override {
        gravity = Vector3(0.0f, -9.8f, 0.0f);  // Gravity in m/s^2
        gravityForce = ParticleGravity(gravity);
        particle = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 1.0f, 1.0f));
    }
};

TEST_F(ParticleGravityTest, UpdateForce) {
    float duration = 1.0f;

    gravityForce.updateForce(particle, duration);

    Vector3 force = particle->getAccumForce();

    // Expected force calculation
    Vector3 expectedForce = gravity * particle->getMass();

    ASSERT_NEAR(force.x, expectedForce.x, EPSILON);
    ASSERT_NEAR(force.y, expectedForce.y, EPSILON);
    ASSERT_NEAR(force.z, expectedForce.z, EPSILON);
}

TEST_F(ParticleGravityTest, UpdateForceWithZeroMass) {
    float duration = 1.0f;

    // Set the particle mass to zero
    particle->setMassReverse(0.0f);

    gravityForce.updateForce(particle, duration);

    Vector3 force = particle->getAccumForce();

    // No force should be applied if the mass is zero
    ASSERT_NEAR(force.x, 0.0f, EPSILON);
    ASSERT_NEAR(force.y, 0.0f, EPSILON);
    ASSERT_NEAR(force.z, 0.0f, EPSILON);
}