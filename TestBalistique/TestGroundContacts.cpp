#include "pch.h"
#include "Particle.h"
#include "Collision/GroundContacts.h"

namespace GroundContactsTest
{
    class TestGroundContacts : public testing::Test {
    protected:
        GroundContacts groundContacts;
        Engine::Particles particles;

        void SetUp() override {
            groundContacts.init(&particles);
        }
    };

    TEST_F(TestGroundContacts, NoContacts)
    {
        // Aucun contact avec le sol, la fonction ne doit rien ajouter à la liste de contacts
        ParticleContact contactArray[5];  // Choisissez la taille en fonction de vos besoins
        int result = groundContacts.addContact(contactArray, 5);

        // Ajoutez des assertions pour vérifier que le nombre de contacts est correct et que les contacts sont nuls
        EXPECT_EQ(result, 0);
        for (int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(contactArray[i].m_particles[0], nullptr);
        }
    }

    TEST_F(TestGroundContacts, SingleContact)
    {
        // Un seul contact avec le sol, la fonction doit ajouter un contact à la liste
        std::shared_ptr<Particle> particle = std::make_shared<Particle>();
        particle->setPosition(Vector3(0.0f, -1.0f, 0.0f));
        particles.push_back(particle);

        ParticleContact contact;
        int result = groundContacts.addContact(&contact, 1);

        // Ajoutez des assertions pour vérifier que le nombre de contacts est correct, que le contact est non nul et que les valeurs du contact sont correctes
        EXPECT_EQ(result, 1);
        EXPECT_EQ(contact.m_particles[0], particle);
        EXPECT_EQ(contact.m_particles[1], nullptr);
        EXPECT_EQ(contact.m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contact.m_penetration, 2.0f);
        EXPECT_FLOAT_EQ(contact.m_restitution, 0.7f);
    }

    TEST_F(TestGroundContacts, AddContact) {
        std::shared_ptr<Particle> particle1 = std::make_shared<Particle>();
        std::shared_ptr<Particle> particle2 = std::make_shared<Particle>();
        std::shared_ptr<Particle> particle3 = std::make_shared<Particle>();

        particle1->setPosition(Vector3(0.0, 0.5, 0.0));
        particle2->setPosition(Vector3(0.0, 0.9, 0.0));
        particle3->setPosition(Vector3(0.0, 5.0, 0.0));

        particles.push_back(particle1);
        particles.push_back(particle2);
        particles.push_back(particle3);
        
        ParticleContact contacts[3];
        
        int contactCount = groundContacts.addContact(contacts, 3);
        
        ASSERT_EQ(contactCount, 2);
        
        //premier contact
        EXPECT_EQ(contacts[0].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[0].m_particles[0], particle1);
        EXPECT_EQ(contacts[0].m_particles[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[0].m_penetration, 0.5f);
        EXPECT_FLOAT_EQ(contacts[0].m_restitution, 0.7f);

        //deuxieme contact
        EXPECT_EQ(contacts[1].m_contactNormal, Vector3(0, -1, 0));
        EXPECT_EQ(contacts[1].m_particles[0], particle2);
        EXPECT_EQ(contacts[1].m_particles[1], nullptr);
        EXPECT_FLOAT_EQ(contacts[1].m_penetration, 0.1f);
        EXPECT_FLOAT_EQ(contacts[1].m_restitution, 0.7f);

        //troisieme contact
        EXPECT_EQ(contacts[2].m_particles[0], nullptr);
    }
}