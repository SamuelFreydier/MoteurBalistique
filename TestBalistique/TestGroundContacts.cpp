#include "pch.h"
#include "Particle.h"
#include "RigidbodyCuboid.h"
#include "Collision/GroundContacts.h"

namespace GroundContactsTest
{
    class TestGroundContacts : public testing::Test {
    protected:
        GroundContacts groundContacts;
        Engine::Particles particles;
        Engine::Rigidbodies rigidbodies;

        void SetUp() override {
            groundContacts.init(&particles, &rigidbodies);
        }
    };

    TEST_F(TestGroundContacts, NoContacts)
    {
        std::shared_ptr<RigidbodyCuboid> rb = std::make_shared<RigidbodyCuboid>();
        rb->setPosition(Vector3(0, 2, 0));
        rigidbodies.push_back(rb);

        Contact contactArray[5];  
        int result = groundContacts.addContact(contactArray, 5);

        EXPECT_EQ(result, 0);
        for (int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(contactArray[i].m_rigidbodies[0], nullptr);
        }
    }

    TEST_F(TestGroundContacts, SingleContact)
    {
        std::shared_ptr<RigidbodyCuboid> rb = std::make_shared<RigidbodyCuboid>();
        rb->setPosition(Vector3(0, 0.5, 0));
        rb->setOrientation(Quaternion((sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, 0));
        rb->calculateDerivedData();
        rigidbodies.push_back(rb);

        Contact contact;
        int result = groundContacts.addContact(&contact, 1);

        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_rigidbodies[0], rb);
        EXPECT_EQ(contact.m_rigidbodies[1], nullptr);
        EXPECT_EQ(contact.m_contactNormal, Vector3(0, -1, 0));
        EXPECT_FLOAT_EQ(contact.m_penetration, 0.33333331f);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.7f);
    }

    TEST_F(TestGroundContacts, FourVertices) 
    {
        std::shared_ptr<RigidbodyCuboid> rb = std::make_shared<RigidbodyCuboid>();
        rb->setPosition(Vector3(0, 0, 0));
        rb->calculateDerivedData();
        rigidbodies.push_back(rb);
        
        Contact contacts[4];
        
        int contactCount = groundContacts.addContact(contacts, 4);
        
        ASSERT_EQ(contactCount, 4);
        
        //premier contact
        EXPECT_EQ(contacts[0].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[0].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[0].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[0].m_penetration, 0.5f);
        EXPECT_FLOAT_EQ(contacts[0].m_restitution, 0.7f);

        //deuxieme contact
        EXPECT_EQ(contacts[1].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[1].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[1].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[1].m_penetration, 0.5f);
        EXPECT_FLOAT_EQ(contacts[1].m_restitution, 0.7f);

        //troisieme contact
        EXPECT_EQ(contacts[2].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[2].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[2].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[2].m_penetration, 0.5f);
        EXPECT_FLOAT_EQ(contacts[2].m_restitution, 0.7f);

        //quatrieme contact
        EXPECT_EQ(contacts[3].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[3].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[3].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[3].m_penetration, 0.5f);
        EXPECT_FLOAT_EQ(contacts[3].m_restitution, 0.7f);
    }

    TEST_F(TestGroundContacts, FourContactsDiffDepths)
    {
        std::shared_ptr<RigidbodyCuboid> rb = std::make_shared<RigidbodyCuboid>();
        rb->setPosition(Vector3(0, 0, 0));
        rb->setOrientation(Quaternion((sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, (sqrt(2) + 1) / 2, 0));
        rb->calculateDerivedData();
        rigidbodies.push_back(rb);

        Contact contacts[4];
        int result = groundContacts.addContact(contacts, 4);

        //premier contact
        EXPECT_EQ(contacts[0].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[0].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[0].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[0].m_penetration, 0.16666669);
        EXPECT_FLOAT_EQ(contacts[0].m_restitution, 0.7f);

        //deuxieme contact
        EXPECT_EQ(contacts[1].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[1].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[1].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[1].m_penetration, 0.49999994);
        EXPECT_FLOAT_EQ(contacts[1].m_restitution, 0.7f);

        //troisieme contact
        EXPECT_EQ(contacts[2].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[2].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[2].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[2].m_penetration, 0.83333331);
        EXPECT_FLOAT_EQ(contacts[2].m_restitution, 0.7f);

        //quatrieme contact
        EXPECT_EQ(contacts[3].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[3].m_rigidbodies[0], rb);
        EXPECT_EQ(contacts[3].m_rigidbodies[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[3].m_penetration, 0.16666669);
        EXPECT_FLOAT_EQ(contacts[3].m_restitution, 0.7f);
    }
}