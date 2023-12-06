#include "pch.h"
#include "Collision/ParticleCable.h"

namespace ParticleCableTest
{
    class TestParticleCable : public testing::Test {
    protected:
        ParticleCable particleCable;
        std::shared_ptr<Rigidbody> rigid1;
        std::shared_ptr<Rigidbody> rigid2;

        void SetUp() override {
            rigid1 = std::make_shared<Rigidbody>();
            rigid2 = std::make_shared<Rigidbody>();

            particleCable.m_rigidbodies[0] = rigid1;
            particleCable.m_rigidbodies[1] = rigid2;

            particleCable.m_maxLength = particleCable.currentLength();
            particleCable.m_restitution = 0.5f;
        }
    };

    TEST_F(TestParticleCable, AddContactNotOverstretched) {
        Contact contact;

        // La longueur actuelle est inférieure à la longueur maximale
        int result = particleCable.addContact(&contact, 1);

        // Assurez-vous qu'aucune collision n'est nécessaire
        EXPECT_EQ(result, 0);
    }

    TEST_F(TestParticleCable, AddContactOverstretched) {
        Contact contact;

        rigid2->setPosition(Vector3(0, 0, 3.0));

        int result = particleCable.addContact(&contact, 1);

        Vector3 normal = -(rigid2->getPosition() - rigid1->getPosition());
        normal.normalize();

        float penetration = particleCable.currentLength() - particleCable.m_maxLength;

        EXPECT_EQ(result, 1);

        EXPECT_EQ(contact.m_rigidbodies[0], rigid1);
        EXPECT_EQ(contact.m_rigidbodies[1], rigid2);

        EXPECT_EQ(contact.m_contactNormal.x, normal.x);
        EXPECT_EQ(contact.m_contactNormal.y, normal.y);
        EXPECT_EQ(contact.m_contactNormal.z, normal.z);

        EXPECT_FLOAT_EQ(contact.m_penetration, penetration);  
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);  
    }
}