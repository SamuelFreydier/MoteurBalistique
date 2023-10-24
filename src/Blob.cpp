#include "Blob.h"


Blob::Blob(const Vector3& sponePosition)
{
	const float particleMass = 5;

	for (int i = 0; i < nbrParticle; i++)
	{
		const float rand1 = (float)(rand() % 1001) / 1000;
		const float rand2 = (float)(rand() % 1001) / 1000;
		const float rand3 = (float)(rand() % 1001) / 1000;
		const float rand4 = (float)(rand() % 1001) / 1000;

		const float particleRadius = 1.5 * rand1 + 0.5;

		const Vector3 particleSponeVelocity = Vector3(rand1 * 2 * sponeRadius - sponeRadius, rand2 * 2 * sponeRadius - sponeRadius);
		const Vector3 particleSponePosition = sponePosition + Vector3(rand3 * 2 * sponeRadius - sponeRadius, rand4 * 2 * sponeRadius - sponeRadius);

		Vector3 particleColor = Vector3(255, 100, 100);
		if (i == 0)
		{
			particleColor.y = 255;
		}

		//Engine::Particles lol = Engine::getInstance()->getParticles();
		Particle* newBlobParticle = new Particle(particleMass, particleRadius, particleSponeVelocity, particleSponePosition, particleColor);
		//Engine::getInstance()->addParticle(newBlobParticle);
		blobParticles.push_back(newBlobParticle);
	}

	initParticleAssociations();
}



Blob::Blob(const std::vector<Particle*>& particlesToAssembly)
{
	blobParticles = particlesToAssembly;

	initParticleAssociations();
}



void Blob::initParticleAssociations()
{
	for (Particle* blobParticle : blobParticles) // On déclare certains couples de particules qui seront reliées par un ressort. Chaque particule doit avoir au moins nbrMaxAssos ressorts
	{
		// par exemple avec 10 particules et nbrMaxAssos, il est possible qu'un groupe de 3 particules attachées entre elles de façon circulaire se 
		// détache du blob principal mais la proba est faible. Il faudrait faire un algo de parcours de graphe pour etre sur que ça n'arrive pas
		// ( = vérification si le graphe est connexe ou non)

		int nbrAssos = 0;

		while (nbrAssos < nbrMaxAssos)
		{
			const int randIndex = rand() % blobParticles.size();

			if (blobParticles[randIndex] != blobParticle)
			{
				bool assoAlreadyExists = false;

				for (ParticleAssociation_t asso : particleAssociations)
				{
					if ((asso.firstParticle == blobParticle && asso.secondParticle == blobParticles[randIndex])
						|| (asso.secondParticle == blobParticle && asso.firstParticle == blobParticles[randIndex]))
					{
						assoAlreadyExists = true;
						nbrAssos++;
						break;
					}
				}

				if (assoAlreadyExists == false)
				{
					particleAssociations.push_back(ParticleAssociation_t({ blobParticle, blobParticles[randIndex] }));
					nbrAssos++;
				}
			}
		}
	}
}



void Blob::eraseDeadParticle(Particle* deadParticle)
{
	bool isParticleInBlob = false;
		
	for (int i = 0; i < blobParticles.size(); i++)
	{
		if (blobParticles[i] == deadParticle)
		{
			isParticleInBlob = true;
			blobParticles.erase(blobParticles.begin() + i);
			break;
		}
	}

	if (blobParticles.size() == 0)
	{
		// TO DO il faut se débrouiller pour enlever le blob entièrement mort de Engine::m_blobs
	}
	else
	{
		if (isParticleInBlob) // si il y a effectivement une particule morte, alors on efface toutes les associations la concernant
		{
			Partuples tempParticleAssociations;

			for (ParticleAssociation_t asso : particleAssociations)
			{
				if (asso.firstParticle != deadParticle && asso.secondParticle != deadParticle)
				{
					tempParticleAssociations.push_back(asso);
				}
			}

			particleAssociations = tempParticleAssociations;
		}
	}
}