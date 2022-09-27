#ifndef H_PARTICLE_MASTER
#define H_PARTICLE_MASTER

#pragma once
#include "ParticleRenderer.h"
#include "InsertionSort.h"

/*
	Holds every particle on screen (that should be rendered or is already rendered),
	checks if particles should update (change world position, rotation etc)
	and rendere them.
	
*/
class ParticleMaster {

public:
	map<ParticleTexture, vector<Particle*>> particles;
	//vector<Particle> particles;
	ParticleRenderer* renderer;

public:
	ParticleMaster(Loader* loader, glm::mat4 projectionMatrix) {
		renderer = new ParticleRenderer(loader, projectionMatrix);
	}

	void update(float deltaTime, PlayerCamera camera) {

		auto mapIt = particles.begin();
		
		while (mapIt != particles.end()) {
			vector<Particle*> batch = mapIt->second;
			auto it = batch.begin();
			bool changed = false;
			while (it != batch.end()) {
				bool stillAlive = (*it)->update(deltaTime, camera);
				if (!stillAlive) {
					it = batch.erase(it);
					if (batch.empty()) {
						mapIt = particles.erase(mapIt);
						changed = true;
					}
				}
				else {
					++it;
				}
			}
			if (!changed)
				++mapIt;
			InsertionSort::sortHighToLow(batch);

		}	
	}

	void renderParticles(PlayerCamera camera) {
		renderer->render(particles, camera);
	}

	void cleanUp() {
		renderer->cleanUp();
	}

	void addParticle(Particle* particle) {
		if (particles[particle->getTexture()].size() != 0) {
			particles[particle->getTexture()].push_back(particle);
		}
		else {
			const std::vector<Particle*> batch = { particle };
			particles[particle->getTexture()] = batch;
		}
	}
};

#endif