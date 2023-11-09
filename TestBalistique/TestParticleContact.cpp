#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleContact.h"

namespace TestParticleContact 
{
    class TestParticleContact : public testing::Test {
    protected:
        ParticleContact contact;
        std::shared_ptr<Particle> particle1;
        std::shared_ptr<Particle> particle2;

        void SetUp() override {
            particle1 = std::make_shared<Particle>();
            particle2 = std::make_shared<Particle>();

            contact.m_particles[0] = particle1;
            contact.m_particles[1] = particle2;
            contact.m_contactNormal = Vector3(0, 0, 1);
            contact.m_penetration = 1.0f; 
            contact.m_restitution = 0.5f; 
        }
    };

    TEST_F(TestParticleContact, NoRelativeVelocity)
    {
        float result = contact.calculateClosingVelocity();
        EXPECT_FLOAT_EQ(result, 0.0f);
    }

    TEST_F(TestParticleContact, EqualAndOppositeVelocities)
    {
        particle1->setVelocity(Vector3(1.0f, 2.0f, 3.0f));
        particle2->setVelocity(Vector3(4.0f, 5.0f, 6.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_FLOAT_EQ(result, -3.0f);
    }

    TEST_F(TestParticleContact, PositiveRelativeVelocity)
    {
        particle1->setVelocity(Vector3(1.0f, 0.0f, 0.0f));
        particle2->setVelocity(Vector3(0.5f, 0.0f, -1.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_GT(result, 0.0f);
    }

    TEST_F(TestParticleContact, NegativeRelativeVelocity)
    {
        particle1->setVelocity(Vector3(1.0f, 0.0f, -3.0f));
        particle2->setVelocity(Vector3(2.0f, 0.0f, 0.0f));

        float result = contact.calculateClosingVelocity();
        EXPECT_LT(result, 0.0f);
    }


    TEST_F(TestParticleContact , ResolveVelocityNoCollision)
    {
        contact.resolveVelocity(1.0f);  

        EXPECT_EQ(particle1->getVelocity(), Vector3(0.0f, 0.0f, 0.0f));
        EXPECT_EQ(particle2->getVelocity(), Vector3(0.0f, 0.0f, 0.0f));
    }

    TEST_F(TestParticleContact , ResolveVelocity)
    {
        particle1->setVelocity(Vector3(2.0f, 0.0f, 0.0f));
        particle2->setVelocity(Vector3(-1.0f, 0.0f, 0.0f));
        contact.m_restitution = 0.8f;  

        contact.resolveVelocity(1.0f);  

        EXPECT_FLOAT_EQ(particle1->getVelocity().x, 2.0f);
        EXPECT_FLOAT_EQ(particle2->getVelocity().x, -1.0f);
    }

    TEST_F(TestParticleContact, ResolveInterpenetration) {
        float initialPosition1 = 1.0f;
        float initialPosition2 = 2.0f;
        float duration = 1.0f; 

        particle1->setPosition(Vector3(0, 0, initialPosition1));
        particle2->setPosition(Vector3(0, 0, initialPosition2));


        contact.resolveInterpenetration(duration);

        EXPECT_FLOAT_EQ(particle1->getPosition().getZ(), 0.5f);
        EXPECT_FLOAT_EQ(particle2->getPosition().getZ(), 2.5f);
    }

    TEST_F(TestParticleContact, ResolveNoInterpenetration) {
        float initialPosition1 = 1.0f;
        float initialPosition2 = 2.0f;
        float duration = 1.0f;

        contact.m_penetration = 0.0f;

        particle1->setPosition(Vector3(0, 0, initialPosition1));
        particle2->setPosition(Vector3(0, 0, initialPosition2));


        contact.resolveInterpenetration(duration);

        EXPECT_FLOAT_EQ(particle1->getPosition().getZ(), initialPosition1);
        EXPECT_FLOAT_EQ(particle2->getPosition().getZ(), initialPosition2);
    }
}