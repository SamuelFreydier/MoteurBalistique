#include "pch.h"
#include "Particle.h"
#include "Collision/ParticleCable.h"

// D�finissez une classe de test pour ParticleCable
class ParticleCableTest : public testing::Test {
protected:
    ParticleCable particleCable;
    std::shared_ptr<Particle> particle1;
    std::shared_ptr<Particle> particle2;

    void SetUp() override {
        particle1 = std::make_shared<Particle>();
        particle2 = std::make_shared<Particle>();

        particleCable.m_particles[0] = particle1;
        particleCable.m_particles[1] = particle2;
    }
};

TEST_F(ParticleCableTest, AddContactNotOverstretched) {
    ParticleContact contact;

    // La longueur actuelle est inf�rieure � la longueur maximale
    int result = particleCable.addContact(&contact, 1);

    // Assurez-vous qu'aucune collision n'est n�cessaire
    EXPECT_EQ(result, 0);
}

TEST_F(ParticleCableTest, AddContactOverstretched) {
    ParticleContact contact;

    // Augmentez la longueur du c�ble au-dessus de la longueur maximale
    particle2->setPosition(Vector3(0, 0, 3.0));

    // La longueur actuelle est sup�rieure � la longueur maximale
    int result = particleCable.addContact(&contact, 1);

    // Assurez-vous qu'une collision est n�cessaire
    EXPECT_EQ(result, 1);

    // V�rifiez les d�tails de la collision (par exemple, le vecteur normal)
    EXPECT_EQ(contact.m_particles[0], particle1);
    EXPECT_EQ(contact.m_particles[1], particle2);
    EXPECT_FLOAT_EQ(contact.m_penetration, 1.0f);  // Longueur actuelle - Longueur maximale
    EXPECT_FLOAT_EQ(contact.m_restitution, 0.5f);  // Assurez-vous de la valeur correcte
}