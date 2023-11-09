#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleAirFriction.h"

namespace AirFrictionTest
{
    const float EPSILON = 1e-6;

    class TestParticleAirFriction : public testing::Test {
    protected:
        ParticleAirFriction particleAirFriction;
        std::shared_ptr<Particle> particle;

        void SetUp() override {
            Vector3 worldWind(1.0f, 2.0f, 3.0f);
            particleAirFriction = ParticleAirFriction(worldWind);
            particle = std::make_shared<Particle>(1.0f, 1.0f, Vector3(2.0f, 2.0f, 2.0f));
        }
    };

    TEST_F(TestParticleAirFriction, UpdateForce) {
        float secondsElapsedSincePreviousUpdate = 1.0f;

        particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

        Vector3 expectedForce = Vector3(0.00057392572f, 0.00114785144f, 0.00172177716f);

        EXPECT_FLOAT_EQ(particle->getAccumForce().x, expectedForce.x, EPSILON);
        EXPECT_FLOAT_EQ(particle->getAccumForce().y, expectedForce.y, EPSILON);
        EXPECT_FLOAT_EQ(particle->getAccumForce().z, expectedForce.z, EPSILON);
    }

    TEST_F(TestParticleAirFriction, GlitchDetection) {
        float secondsElapsedSincePreviousUpdate = 1.0f;

        particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

        bool isFrictionGlitch = particle->isGlitchedFriction();
        ASSERT_FALSE(isFrictionGlitch);

        particle->setIsGlitchedFriction(false);
        particle->setVelocity(Vector3(0.5f, 0.5f, 0.5f));

        particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

        isFrictionGlitch = particle->isGlitchedFriction();
        ASSERT_TRUE(isFrictionGlitch);

        Vector3 expectedVelocity = Vector3(1.0f, 2.0f, 3.0f);
        ASSERT_EQ(particle->getVelocity(), expectedVelocity);
    }
}