#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleCable.h"

namespace ParticleCableTest
{
    class TestParticleCable : public testing::Test {
    protected:
        ParticleCable particleCable;
        std::shared_ptr<Particle> particle1;
        std::shared_ptr<Particle> particle2;

        void SetUp() override {
            particle1 = std::make_shared<Particle>();
            particle2 = std::make_shared<Particle>();

            particleCable.m_particles[0] = particle1;
            particleCable.m_particles[1] = particle2;

            particleCable.m_maxLength = particleCable.currentLength();
            particleCable.m_restitution = 0.5f;
        }
    };

    TEST_F(TestParticleCable, AddContactNotOverstretched) {
        ParticleContact contact;

        // La longueur actuelle est inférieure à la longueur maximale
        int result = particleCable.addContact(&contact, 1);

        // Assurez-vous qu'aucune collision n'est nécessaire
        EXPECT_EQ(result, 0);
    }

    TEST_F(TestParticleCable, AddContactOverstretched) {
        ParticleContact contact;

        particle2->setPosition(Vector3(0, 0, 3.0));

        int result = particleCable.addContact(&contact, 1);

        Vector3 normal = -(particle2->getPosition() - particle1->getPosition());
        normal.normalize();

        float penetration = particleCable.currentLength() - particleCable.m_maxLength;

        EXPECT_EQ(result, 1);

        EXPECT_EQ(contact.m_particles[0], particle1);
        EXPECT_EQ(contact.m_particles[1], particle2);

        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);

        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);  
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);  
    }
}