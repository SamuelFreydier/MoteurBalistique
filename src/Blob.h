#ifndef BLOB_H
#define BLOB_H

#include "Particle.h"

// La classe gérant les blobs
class Blob
{
	public:
		struct ParticleAssociation_t
		{
			Particle* firstParticle;
			Particle* secondParticle;
		};

		typedef std::vector< ParticleAssociation_t> Partuples;

	private :
		std::vector<Particle*> blobParticles;
		Partuples particleAssociations;

		const int nbrParticle = 10;
		const float sponeRadius = 5;


	public:
		Blob(const Vector3& sponePosition = Vector3());
		Blob(const std::vector<Particle*>& particlesToAssembly);
		// TO DO destructeur ~Blob()

		void initParticleAssociations();
		void eraseDeadParticle(Particle* deadParticle);

		const std::vector<Particle*>& getBlobParticles() { return blobParticles; };
		const Partuples& getParticleAssociations() { return particleAssociations; };

		//void move();

};

#endif