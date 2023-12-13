#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleContact.h"

namespace TestParticleContact 
{
    class TestParticleContact : public testing::Test {
    protected:
        Contact contact;
        std::shared_ptr<Rigidbody> rb1;
        std::shared_ptr<Rigidbody> rb2;

        void SetUp() override {
            rb1 = std::make_shared<Rigidbody>();
            rb2 = std::make_shared<Rigidbody>();

            contact.m_rigidbodies[0] = rb1;
            contact.m_rigidbodies[1] = rb2;
            contact.m_contactNormal = Vector3(0, 0, 1);
            contact.m_penetration = 1.0f; 
            contact.m_restitution = 0.5f; 
        }
    };

    /*
    TEST_F(TestParticleContact, NoRelativeVelocity)
    {
        float result = contact.calculateClosingVelocity();
        EXPECT_FLOAT_EQ(result, 0.0f);
    }

    TEST_F(TestParticleContact, EqualAndOppositeVelocities)
    {
        rb1->setVelocity(Vector3(1.0f, 2.0f, 3.0f));
        rb2->setVelocity(Vector3(4.0f, 5.0f, 6.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_FLOAT_EQ(result, -3.0f);
    }

    TEST_F(TestParticleContact, PositiveRelativeVelocity)
    {
        rb1->setVelocity(Vector3(1.0f, 0.0f, 0.0f));
        rb2->setVelocity(Vector3(0.5f, 0.0f, -1.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_GT(result, 0.0f);
    }

    TEST_F(TestParticleContact, NegativeRelativeVelocity)
    {
        rb1->setVelocity(Vector3(1.0f, 0.0f, -3.0f));
        rb2->setVelocity(Vector3(2.0f, 0.0f, 0.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_LT(result, 0.0f);
    }


    TEST_F(TestParticleContact , ResolveVelocityNoCollision)
    {
        contact.resolveVelocity(1.0f);  

        EXPECT_EQ(rb1->getVelocity(), Vector3(0.0f, 0.0f, 0.0f));
        EXPECT_EQ(rb2->getVelocity(), Vector3(0.0f, 0.0f, 0.0f));
    }

    TEST_F(TestParticleContact , ResolveVelocity)
    {
        rb1->setVelocity(Vector3(2.0f, 0.0f, 0.0f));
        rb2->setVelocity(Vector3(-1.0f, 0.0f, 0.0f));
        contact.m_restitution = 0.8f;  

        contact.resolveVelocity(1.0f);  

        EXPECT_FLOAT_EQ(rb1->getVelocity().x, 2.0f);
        EXPECT_FLOAT_EQ(rb2->getVelocity().x, -1.0f);
    }

    TEST_F(TestParticleContact, ResolveInterpenetration) {
        float initialPosition1 = 1.0f;
        float initialPosition2 = 2.0f;
        float duration = 1.0f; 

        rb1->setPosition(Vector3(0, 0, initialPosition1));
        rb2->setPosition(Vector3(0, 0, initialPosition2));


        contact.resolveInterpenetration(duration);

        EXPECT_FLOAT_EQ(rb1->getPosition().getZ(), 0.5f);
        EXPECT_FLOAT_EQ(rb2->getPosition().getZ(), 2.5f);
    }

    TEST_F(TestParticleContact, ResolveNoInterpenetration) {
        float initialPosition1 = 1.0f;
        float initialPosition2 = 2.0f;
        float duration = 1.0f;

        contact.m_penetration = 0.0f;

        rb1->setPosition(Vector3(0, 0, initialPosition1));
        rb2->setPosition(Vector3(0, 0, initialPosition2));


        contact.resolveInterpenetration(duration);

        EXPECT_FLOAT_EQ(rb1->getPosition().getZ(), initialPosition1);
        EXPECT_FLOAT_EQ(rb2->getPosition().getZ(), initialPosition2);
    }*/
}