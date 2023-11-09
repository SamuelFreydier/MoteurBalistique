#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleRod.h"

class ParticleRodTest : public testing::Test {
protected:
    ParticleRod rod;
    std::shared_ptr<Particle> particle1;
    std::shared_ptr<Particle> particle2;

    void SetUp() override {
        particle1 = std::make_shared<Particle>();
        particle2 = std::make_shared<Particle>();
        rod.m_particles[0] = particle1;
        rod.m_particles[1] = particle2;
        rod.m_length = 1.0f;  // Exemple de longueur de la tige.
    }
};

TEST_F(ParticleRodTest, AddContact_NoCollision) {
    // La tige a la longueur correcte, il ne devrait pas y avoir de collision
    ParticleContact contact;
    int limit = 1; 

    int result = rod.addContact(&contact, limit);

    EXPECT_EQ(result, 0);
}

TEST_F(ParticleRodTest, AddContact_ExtensionCollision) {
    // La tige est trop étendue, il devrait y avoir une collision d'extension
    particle1->setPosition(Vector3(0, 0, 0));
    particle2->setPosition(Vector3(0, 0, 1.5));  // Distance > longueur de la tige
    ParticleContact contact;
    int limit = 1; 

    int result = rod.addContact(&contact, limit);

    EXPECT_EQ(result, 1);
    EXPECT_EQ(contact.m_contactNormal, Vector3(0, 0, 1));
    EXPECT_FLOAT_EQ(contact.m_penetration, 0.5);
}

TEST_F(ParticleRodTest, AddContact_CompressionCollision) {
    // La tige est trop courte, il devrait y avoir une collision de compression
    particle1->setPosition(Vector3(0, 0, 0));
    particle2->setPosition(Vector3(0, 0, 0.5)); 
    ParticleContact contact;
    int limit = 1;

    int result = rod.addContact(&contact, limit);

    EXPECT_EQ(result, 1);
    EXPECT_EQ(contact.m_contactNormal, Vector3(0, 0, -1));
    EXPECT_FLOAT_EQ(contact.m_penetration, 0.5);
}