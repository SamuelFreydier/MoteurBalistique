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
			float associationElasticity = 15.f;
			float associationRestLength = 5.f;
			float associationRestitution = 0.1f;
		};

		typedef std::vector< ParticleAssociation_t> Partuples;

	private :
		std::vector<std::shared_ptr<Particle>> m_blobParticles;
		Partuples m_particleAssociations;

		const int NBR_PARTICLE = 10;
		const float SPAWN_RADIUS = 5;

	public:
		Blob(const Vector3& spawnPosition = Vector3());
		Blob(const std::vector<std::shared_ptr<Particle>>& particlesToAssembly);

		void initParticleAssociations();
		void eraseDeadParticle(std::shared_ptr<Particle> deadParticle);

		const std::vector<std::shared_ptr<Particle>>& getBlobParticles() { return m_blobParticles; }
		std::vector<std::shared_ptr<Particle>>& getBlobParticlesMutable() { return m_blobParticles; }
		void addParticle( std::shared_ptr<Particle> particle ) { m_blobParticles.push_back( particle ); }
		const Partuples& getParticleAssociations() { return m_particleAssociations; }

		void clearParticleAssociations() { m_particleAssociations.clear(); }
		void clearBlobParticles() { m_blobParticles.clear(); }

		// void move();
};

#endif