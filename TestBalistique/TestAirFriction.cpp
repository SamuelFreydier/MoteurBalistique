#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleAirFriction.h"

const float EPSILON = 1e-6;

// Test fixture class for ParticleAirFriction
class ParticleAirFrictionTest : public testing::Test {
protected:
    ParticleAirFriction particleAirFriction;
    std::shared_ptr<Particle> particle;

    void SetUp() override {
        Vector3 worldWind(1.0f, 2.0f, 3.0f);
        particleAirFriction = ParticleAirFriction(worldWind);
        particle = std::make_shared<Particle>(1.0f, 1.0f, Vector3(2.0f, 2.0f, 2.0f));
    }
};

TEST_F(ParticleAirFrictionTest, UpdateForce) {
    float secondsElapsedSincePreviousUpdate = 1.0f;

    particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

    Vector3 expectedForce = Vector3(0.00057392572f, 0.00114785144f, 0.00172177716f);

    ASSERT_NEAR(particle->getAccumForce().x, expectedForce.x, EPSILON);
    ASSERT_NEAR(particle->getAccumForce().y, expectedForce.y, EPSILON);
    ASSERT_NEAR(particle->getAccumForce().z, expectedForce.z, EPSILON);
}

TEST_F(ParticleAirFrictionTest, GlitchDetection) {
    float secondsElapsedSincePreviousUpdate = 1.0f;

    particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

    bool isFrictionGlitch = particle->isGlitchedFriction();
    ASSERT_FALSE(isFrictionGlitch);

    // Reset the glitch flag and change the velocity
    particle->setIsGlitchedFriction(false);
    particle->setVelocity(Vector3(0.5f, 0.5f, 0.5f));

    particleAirFriction.updateForce(particle, secondsElapsedSincePreviousUpdate);

    isFrictionGlitch = particle->isGlitchedFriction();
    ASSERT_TRUE(isFrictionGlitch);

    Vector3 expectedVelocity = Vector3(1.0f, 2.0f, 3.0f);
    ASSERT_EQ(particle->getVelocity(), expectedVelocity);
}