#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleRod.h"

namespace ParticleRodTest
{
    class TestParticleRod : public testing::Test {
    protected:
        ParticleRod rod;
        std::shared_ptr<Particle> particle1;
        std::shared_ptr<Particle> particle2;

        void SetUp() override {
            particle1 = std::make_shared<Particle>();
            particle2 = std::make_shared<Particle>();

            particle2->setPosition(Vector3(0.0f, 0.0f, 1.0f));

            rod.m_particles[0] = particle1;
            rod.m_particles[1] = particle2;
            rod.m_length = rod.currentLength();
        }
    };

    TEST_F(TestParticleRod, AddContact_NoCollision) {
        Contact contact;
        int limit = 1;

        int result = rod.addContact(&contact, limit);

        EXPECT_EQ(result, 0);
    }

    TEST_F(TestParticleRod, AddContact_ExtensionCollision) {
        particle1->setPosition(Vector3(0, 0, 0));
        particle2->setPosition(Vector3(0, 0, 1.5)); 
        Contact contact;
        int limit = 1;

        int result = rod.addContact(&contact, limit);
        float penetration = rod.currentLength() - rod.m_length;
        Vector3 normal = particle2->getPosition() - particle1->getPosition();
        normal.normalize();

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);
        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);
    }

    TEST_F(TestParticleRod, AddContact_CompressionCollision) {
        particle1->setPosition(Vector3(0, 0, 0));
        particle2->setPosition(Vector3(0, 0, 0.5));
        Contact contact;
        int limit = 1;

        int result = rod.addContact(&contact, limit);
        float penetration = rod.m_length - rod.currentLength();
        Vector3 normal = -(particle2->getPosition() - particle1->getPosition());
        normal.normalize();

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);
        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);
    }
}