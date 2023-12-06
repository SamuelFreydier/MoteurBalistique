#include "pch.h"
#include "Collision/ParticleRod.h"

namespace ParticleRodTest
{
    class TestParticleRod : public testing::Test {
    protected:
        ParticleRod rod;
        std::shared_ptr<Rigidbody> rb1;
        std::shared_ptr<Rigidbody> rb2;

        void SetUp() override {
            rb1 = std::make_shared<Rigidbody>();
            rb2 = std::make_shared<Rigidbody>();

            rb2->setPosition(Vector3(0.0f, 0.0f, 1.0f));

            rod.m_rigidbodies[0] = rb1;
            rod.m_rigidbodies[1] = rb2;
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
        rb1->setPosition(Vector3(0, 0, 0));
        rb2->setPosition(Vector3(0, 0, 1.5)); 
        Contact contact;
        int limit = 1;

        int result = rod.addContact(&contact, limit);
        float penetration = rod.currentLength() - rod.m_length;
        Vector3 normal = rb2->getPosition() - rb1->getPosition();
        normal.normalize();

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);
        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);
    }

    TEST_F(TestParticleRod, AddContact_CompressionCollision) {
        rb1->setPosition(Vector3(0, 0, 0));
        rb2->setPosition(Vector3(0, 0, 0.5));
        Contact contact;
        int limit = 1;

        int result = rod.addContact(&contact, limit);
        float penetration = rod.m_length - rod.currentLength();
        Vector3 normal = -(rb2->getPosition() - rb1->getPosition());
        normal.normalize();

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);
        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);
    }
}