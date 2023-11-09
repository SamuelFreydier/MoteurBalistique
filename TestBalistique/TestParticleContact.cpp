#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleContact.h"

class ParticleContactTest : public testing::Test {
protected:
    ParticleContact contact;
    std::shared_ptr<Particle> particle1;
    std::shared_ptr<Particle> particle2;

    void SetUp() override {
        particle1 = std::make_shared<Particle>();
        particle2 = std::make_shared<Particle>();
        contact.m_particles[0] = particle1;
        contact.m_particles[1] = particle2;
        contact.m_contactNormal = Vector3(0, 0, 1);  // Exemple d'une normale de collision (vers le haut).
        contact.m_penetration = 1.0f;  // Exemple de pénétration.
        contact.m_restitution = 0.5f;  // Exemple de coefficient de restitution.
    }
};

TEST_F(ParticleContactTest, ResolveVelocity) {
    float initialVelocity1 = 2.0f;
    float initialVelocity2 = 1.0f;

    particle1->setVelocity(Vector3(0, 0, initialVelocity1));
    particle2->setVelocity(Vector3(0, 0, initialVelocity2));

    float duration = 1.0f;  // Exemple de durée.

    contact.resolveVelocity(duration);

    // Vérifie que la résolution de la vélocité s'est déroulée correctement.
    EXPECT_FLOAT_EQ(particle1->getVelocity().getZ(), 0.5f * initialVelocity1);
    EXPECT_FLOAT_EQ(particle2->getVelocity().getZ(), 0.5f * initialVelocity2);
}

TEST_F(ParticleContactTest, ResolveInterpenetration) {
    float initialPosition1 = 1.0f;
    float initialPosition2 = 2.0f;

    particle1->setPosition(Vector3(0, 0, initialPosition1));
    particle2->setPosition(Vector3(0, 0, initialPosition2));

    float duration = 1.0f;  // Exemple de durée.

    contact.resolveInterpenetration(duration);

    // Vérifie que la résolution de l'interpénétration s'est déroulée correctement.
    EXPECT_FLOAT_EQ(particle1->getPosition().getZ(), 1.25f);
    EXPECT_FLOAT_EQ(particle2->getPosition().getZ(), 1.75f);
}