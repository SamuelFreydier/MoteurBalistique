#ifndef BLOB_H
#define BLOB_H

#include "Particle.h"

// La classe gérant les blobs
class Blob
{
	public:
		struct ParticleAssociation_t
		{
			std::shared_ptr<Particle> firstParticle;
			std::shared_ptr<Particle> secondParticle;
		};

		typedef std::vector< ParticleAssociation_t> Partuples;

	private :
		std::vector<std::shared_ptr<Particle>> blobParticles;
		Partuples particleAssociations;

		const int nbrParticle = 10;
		const float sponeRadius = 5;


	public:
		Blob(const Vector3& sponePosition = Vector3());
		Blob(const std::vector<std::shared_ptr<Particle>>& particlesToAssembly);
		// TO DO destructeur ~Blob()

		void initParticleAssociations();
		void eraseDeadParticle(std::shared_ptr<Particle> deadParticle);

		const std::vector<std::shared_ptr<Particle>>& getBlobParticles() { return blobParticles; };
		const Partuples& getParticleAssociations() { return particleAssociations; };

		//void move();

};

#endif