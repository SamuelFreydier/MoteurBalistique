#include "pch.h"
#include "Particle.h"
#include "ParticleForceGenerators/ParticleBungee.h"


namespace ParticleBungeeTest
{
    const float EPSILON = 1e-6;

    TEST(TestParticleBungee, UpdateForceNotCompressed) {
        float duration = 1.0f;
        std::shared_ptr<Particle> particle1 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 0.0f, 0.0f));
        std::shared_ptr<Particle> particle2 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(0.0f, 0.0f, 0.0f));
        ParticleBungee bungee = ParticleBungee(particle2, 1.0f, 2.0f);

        bungee.updateForce(particle1, duration);

        Vector3 force = particle1->getAccumForce();

        Vector3 springVector = particle1->getPosition() - particle2->getPosition();
        float length = springVector.norm();
        float expectedNorm = -bungee.getSpringConstant() * (length - bungee.getRestLength());
        Vector3 expectedForce = springVector.normalized() * expectedNorm;

        ASSERT_NEAR(force.x, expectedForce.x, EPSILON);
        ASSERT_NEAR(force.y, expectedForce.y, EPSILON);
        ASSERT_NEAR(force.z, expectedForce.z, EPSILON);
    }

    TEST(TestParticleBungee, UpdateForceCompressed) {
        float duration = 1.0f;
        std::shared_ptr<Particle> particle1 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(1.0f, 0.0f, 0.0f));
        std::shared_ptr<Particle> particle2 = std::make_shared<Particle>(1.0f, 2.0f, Vector3(0.0f, 0.0f, 0.0f));
        ParticleBungee bungee = ParticleBungee(particle2, 1.0f, 2.0f);

        particle1->setPosition(Vector3(0.0f, 0.0f, 0.0f));

        bungee.updateForce(particle1, duration);

        Vector3 force = particle1->getAccumForce();

        ASSERT_NEAR(force.x, 0.0f, EPSILON);
        ASSERT_NEAR(force.y, 0.0f, EPSILON);
        ASSERT_NEAR(force.z, 0.0f, EPSILON);
    }
}