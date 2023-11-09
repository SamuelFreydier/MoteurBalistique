#include "pch.h"
#include "Particle.h"
#include "Collision/GroundContacts.h"

//Définissez une classe de test pour GroundContacts
class GroundContactsTest : public testing::Test {
protected:
    GroundContacts groundContacts;
    Engine::Particles particles;

    void SetUp() override {
        groundContacts.init(&particles);
    }
};

TEST_F(GroundContactsTest, AddContact) {
    // Créez quelques particules pour le test
    std::shared_ptr<Particle> particle1 = std::make_shared<Particle>();
    std::shared_ptr<Particle> particle2 = std::make_shared<Particle>(1.0f, 0.1f, Vector3(0, -0.5, 0));
    std::shared_ptr<Particle> particle3 = std::make_shared<Particle>(1.0f, 0.1f, Vector3(0, -1.0, 0), Vector3(1.0, 2.0, 3.0));


    // Ajoutez les particules à la liste des particules
    particles.push_back(particle1);
    particles.push_back(particle2);
    particles.push_back(particle3);

    // Créez un contact pour tester la fonction
    ParticleContact contact;

    // Appelez la fonction addContact pour détecter les contacts avec le sol
    //int contactCount = groundContacts.addContact(&contact, 3);
    //
    //// Vérifiez que le nombre de contacts est correct
    //ASSERT_EQ(contactCount, 3);
    //
    //EXPECT_EQ(contact.m_contactNormal, Vector3(0, -1, 0));
    //EXPECT_EQ(contact.m_particles[0], particle1);
    //EXPECT_EQ(contact.m_particles[1], nullptr);
    //EXPECT_FLOAT_EQ(contact.m_penetration, 0.1f);
    //EXPECT_FLOAT_EQ(contact.m_restitution, 0.7f);
}