#include "Blob.h"

/**
 * @brief Construction d'un blob de NBR_PARTICLE à la position spawnPosition
 * @param spawnPosition 
*/
Blob::Blob(const Vector3& spawnPosition)
{
	const float particleMass = 5;

	for (int i = 0; i < NBR_PARTICLE; i++)
	{
		const float rand1 = (float)(rand() % 1001) / 1000;
		const float rand2 = (float)(rand() % 1001) / 1000;
		const float rand3 = (float)(rand() % 1001) / 1000;
		const float rand4 = (float)(rand() % 1001) / 1000;

		const float particleRadius = 1.5 * rand1 + 0.5;

		const Vector3 particleSpawnVelocity = Vector3(rand1 * 2 * SPAWN_RADIUS - SPAWN_RADIUS, rand2 * 2 * SPAWN_RADIUS - SPAWN_RADIUS);
		const Vector3 particleSpawnPosition = spawnPosition + Vector3(rand3 * 2 * SPAWN_RADIUS - SPAWN_RADIUS, rand4 * 2 * SPAWN_RADIUS - SPAWN_RADIUS);

		Vector3 particleColor = Vector3(255, 100, 100);
		if (i == 0)
		{
			particleColor.y = 255;
		}

		//Engine::Particles lol = Engine::getInstance()->getParticles();
		std::shared_ptr<Particle> newBlobParticle = std::make_shared<Particle>(particleMass, particleRadius, particleSpawnVelocity, particleSpawnPosition, particleColor);

		//Engine::getInstance()->addParticle(newBlobParticle);
		m_blobParticles.push_back(newBlobParticle);
	}

	initParticleAssociations();
}


/**
 * @brief Construction d'un blob à partir d'un ensemble de particules
 * @param particlesToAssembly 
*/
Blob::Blob(const std::vector<std::shared_ptr<Particle>>& particlesToAssembly)
{
	m_blobParticles = particlesToAssembly;

	initParticleAssociations();
}


/**
 * @brief Initialisation des associations entre les particules du blob
*/
void Blob::initParticleAssociations()
{
	// Très moche mais ça sert à ne pas avoir de boucle infinie
	int nbrMaxAssos;
	if (m_blobParticles.size() >= 3)
	{
		nbrMaxAssos = 2;
	}
	else if (m_blobParticles.size() == 2)
	{
		nbrMaxAssos = 1;
	}
	else
	{
		nbrMaxAssos = 0;
	}

	for ( std::shared_ptr<Particle> blobParticle : m_blobParticles) // On déclare certains couples de particules qui seront reliées par un ressort. Chaque particule doit avoir au moins nbrMaxAssos ressorts
	{
		// par exemple avec 10 particules et nbrMaxAssos, il est possible qu'un groupe de 3 particules attachées entre elles de façon circulaire se 
		// détache du blob principal mais la proba est faible. Il faudrait faire un algo de parcours de graphe pour etre sur que ça n'arrive pas
		// ( = vérification si le graphe est connexe ou non)

		int nbrAssos = 0;

		while (nbrAssos < nbrMaxAssos)
		{
			const int randIndex = rand() % m_blobParticles.size();

			if (m_blobParticles[randIndex] != blobParticle)
			{
				bool assoAlreadyExists = false;

				for (ParticleAssociation_t asso : m_particleAssociations)
				{
					if ((asso.firstParticle == blobParticle && asso.secondParticle == m_blobParticles[randIndex])
						|| (asso.secondParticle == blobParticle && asso.firstParticle == m_blobParticles[randIndex]))
					{
						assoAlreadyExists = true;
						nbrAssos++;
						break;
					}
				}

				if (assoAlreadyExists == false)
				{
					m_particleAssociations.push_back(ParticleAssociation_t({ blobParticle, m_blobParticles[randIndex] }));
					nbrAssos++;
				}
			}
		}
	}
}


/**
 * @brief Supprime les particules ayant été supprimées au préalable par Engine du blob
 * @param deadParticle 
*/
void Blob::eraseDeadParticle( std::shared_ptr<Particle> deadParticle)
{
	bool isParticleInBlob = false;
		
	for (int i = 0; i < m_blobParticles.size(); i++)
	{
		if (m_blobParticles[i] == deadParticle)
		{
			isParticleInBlob = true;
			m_blobParticles.erase(m_blobParticles.begin() + i);
			break;
		}
	}

	if (m_blobParticles.size() == 0)
	{
		// TO DO il faut se débrouiller pour enlever le blob entièrement mort de Engine::m_blobs
	}
	else
	{
		if (isParticleInBlob) // si il y a effectivement une particule morte, alors on efface toutes les associations la concernant
		{
			Partuples tempParticleAssociations;

			for (ParticleAssociation_t asso : m_particleAssociations)
			{
				if (asso.firstParticle != deadParticle && asso.secondParticle != deadParticle)
				{
					float sumRadius = asso.firstParticle->getRadius() + asso.secondParticle->getRadius();
					asso.associationRestLength = sumRadius;
					tempParticleAssociations.push_back(asso);
				}
			}

			m_particleAssociations = tempParticleAssociations;
		}
	}
}