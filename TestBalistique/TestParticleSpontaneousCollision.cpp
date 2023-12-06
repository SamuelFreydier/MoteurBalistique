#include "pch.h"
#include "RigidbodyCuboid.h"
#include "Collision/ParticleSpontaneousCollision.h"

namespace ParticleSpontaneousCollisionTest
{
    const Vector3 EPSILON_F = 1e-4;
    const Vector3 EPSILON_V = Vector3(1e-4, 1e-4, 1e-4);

    class TestParticleSpontaneousCollision : public testing::Test {
    protected:
        SpontaneousCollision collision;
        std::shared_ptr<RigidbodyCuboid> rb1;
        std::shared_ptr<RigidbodyCuboid> rb2;

        void SetUp() override {
            rb1 = std::make_shared<RigidbodyCuboid>();
            rb2 = std::make_shared<RigidbodyCuboid>();
            collision.m_rigidbodies[0] = rb1;
            collision.m_rigidbodies[1] = rb2;
            collision.m_restitution = 0.5f; 
        }
    };

    TEST_F(TestParticleSpontaneousCollision, AddContact_NoCollision) {
        rb1->setPosition(Vector3(0, 0, 0));
        rb2->setPosition(Vector3(0, 0, 2.0)); 

        rb1->calculateDerivedData();
        rb2->calculateDerivedData();

        Contact contact;
        int limit = 1; 

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 0);
    }

    TEST_F(TestParticleSpontaneousCollision, AddContact_Collision_Sommet2Face1) {
        rb1->setPosition(Vector3(0, 0, 0));

        rb2->setPosition(Vector3(1, 0, 0));
        rb2->setOrientation(Quaternion((sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, 0));


        rb1->calculateDerivedData();
        rb2->calculateDerivedData();

        Contact contact;
        int limit = 1; 

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal, rb1->getAxis(0));
        EXPECT_EQ(contact.m_contactPoint, rb2->getVerticesGlobalPosition()[7]);
        EXPECT_FLOAT_EQ(contact.m_penetration, 0.33333325, EPSILON);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);
    }

    TEST_F(TestParticleSpontaneousCollision, AddContact_Collision_Sommet1Face2) {
        rb1->setPosition(Vector3(0, 0, 0));
        rb1->setOrientation(Quaternion((sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, 0));

        rb2->setPosition(Vector3(1, 0, 0));

        rb1->calculateDerivedData();
        rb2->calculateDerivedData();

        Contact contact;
        int limit = 1;

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal, rb2->getAxis(0) * (-1.0));
        EXPECT_EQ(contact.m_contactPoint, rb1->getVerticesGlobalPosition()[0]);
        EXPECT_FLOAT_EQ(contact.m_penetration, 0.33333325, EPSILON_F);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);
    }

    TEST_F(TestParticleSpontaneousCollision, AddContact_Collision_Aretes) {
        rb1->setPosition(Vector3(0, 0, 0));
        rb2->setPosition(Vector3(0.75, 0.75, 0));
        rb2->setOrientation(Quaternion((sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, 0));

        rb1->calculateDerivedData();
        rb2->calculateDerivedData();

        Contact contact;
        int limit = 1;

        int result = collision.addContact(&contact, limit);

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_contactNormal, Vector3(0.707106769, 0.707106888, 0), EPSILON_V);
        EXPECT_EQ(contact.m_contactPoint, Vector3(0.374999881, 0.375000119, 8.94069743e-08), EPSILON_V);
        EXPECT_FLOAT_EQ(contact.m_penetration, 0.3535533);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);
    }
}