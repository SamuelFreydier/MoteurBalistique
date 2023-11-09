#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleSpontaneousCollision.h"

namespace ParticleSpontaneousCollisionTest
{
    class TestParticleSpontaneousCollision : public testing::Test {
    protected:
        ParticleSpontaneousCollision collision;
        std::shared_ptr<Particle> particle1;
        std::shared_ptr<Particle> particle2;

        void SetUp() override {
            particle1 = std::make_shared<Particle>();
            particle2 = std::make_shared<Particle>();
            collision.m_particles[0] = particle1;
            collision.m_particles[1] = particle2;
            collision.m_restitution = 0.5f; 
        }
    };

    TEST_F(TestParticleSpontaneousCollision, AddContact_NoCollision) {
        particle1->setPosition(Vector3(0, 0, 0));
        particle2->setPosition(Vector3(0, 0, 2.0)); 

        ParticleContact contact;
        int limit = 1; 

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 0);
    }

    TEST_F(TestParticleSpontaneousCollision, AddContact_Collision) {
        particle1->setPosition(Vector3(0, 0, 0));
        particle2->setPosition(Vector3(0, 0, 1.5)); 

        ParticleContact contact;
        int limit = 1; 

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal, Vector3(0, 0, 1));
        EXPECT_FLOAT_EQ(contact.m_penetration, 0.5);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);
    }
}