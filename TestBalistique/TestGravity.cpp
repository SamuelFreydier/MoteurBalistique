#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleGravity.h"

namespace ParticleGravityTest
{
    const float EPSILON = 1e-6;

    class TestParticleGravity : public testing::Test {
    protected:
        Gravity gravityForce;
        std::shared_ptr<Particle> particle;
        Vector3 gravity;

        void SetUp() override {
            gravity = Vector3(0.0f, -9.8f, 0.0f);
            gravityForce = Gravity(gravity);
            particle = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 1.0f, 1.0f));
        }
    };

    TEST_F(TestParticleGravity, UpdateForce) {
        float duration = 1.0f;

        gravityForce.updateForce(particle, duration);

        Vector3 force = particle->getAccumForce();

        Vector3 expectedForce = gravity * particle->getMass();

        ASSERT_NEAR(force.x, expectedForce.x, EPSILON);
        ASSERT_NEAR(force.y, expectedForce.y, EPSILON);
        ASSERT_NEAR(force.z, expectedForce.z, EPSILON);
    }

    TEST_F(TestParticleGravity, UpdateForceWithZeroMass) {
        float duration = 1.0f;

        particle->setMassReverse(0.0f);

        gravityForce.updateForce(particle, duration);

        Vector3 force = particle->getAccumForce();

        ASSERT_NEAR(force.x, 0.0f, EPSILON);
        ASSERT_NEAR(force.y, 0.0f, EPSILON);
        ASSERT_NEAR(force.z, 0.0f, EPSILON);
    }
}