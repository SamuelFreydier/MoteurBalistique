#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleSpontaneousCollision.h"

class ParticleSpontaneousCollisionTest : public testing::Test {
protected:
    ParticleSpontaneousCollision collision;
    std::shared_ptr<Particle> particle1;
    std::shared_ptr<Particle> particle2;

    void SetUp() override {
        particle1 = std::make_shared<Particle>();
        particle2 = std::make_shared<Particle>();
        collision.m_particles[0] = particle1;
        collision.m_particles[1] = particle2;
        collision.m_restitution = 0.5f;  // Exemple de coefficient de restitution.
    }
};

TEST_F(ParticleSpontaneousCollisionTest, AddContact_NoCollision) {
    // Les particules sont suffisamment éloignées, il ne devrait pas y avoir de collision.
    particle1->setPosition(Vector3(0, 0, 0));
    particle2->setPosition(Vector3(0, 0, 2.0));  // Exemple de distance suffisante.

    ParticleContact contact;
    int limit = 1;  // Exemple de limite.

    int result = collision.addContact(&contact, limit);

    // Vérifie que le résultat est correct (pas de collision).
    EXPECT_EQ(result, 0);
}

TEST_F(ParticleSpontaneousCollisionTest, AddContact_Collision) {
    // Les particules sont suffisamment proches, il devrait y avoir une collision.
    particle1->setPosition(Vector3(0, 0, 0));
    particle2->setPosition(Vector3(0, 0, 1.5));  // Exemple de distance insuffisante.

    ParticleContact contact;
    int limit = 1;  // Exemple de limite.

    int result = collision.addContact(&contact, limit);

    // Vérifie que le résultat est correct (collision).
    EXPECT_EQ(result, 1);
    // Vérifie que la normale de collision est correcte (pointe vers le haut).
    EXPECT_EQ(contact.m_contactNormal, Vector3(0, 0, 1));
    // Vérifie que la pénétration est correcte (somme des rayons - distance).
    EXPECT_FLOAT_EQ(contact.m_penetration, 0.5);
    // Vérifie que le coefficient de restitution est correct.
    EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);
}