#ifndef H_PARTICLE_SYSTEM
#define H_PARTICLE_SYSTEM

#pragma once
#include "ParticleMaster.h"

/*
	Simulates multiple particles that have certain behaviour in order to make desired effect.
*/
class ParticleSystem {
private:
	float pps;
	float speed;
	float gravityComplient;
	float lifeLength;
	ParticleMaster* master;

	ParticleTexture texture;

public:
	ParticleSystem(ParticleMaster* master, ParticleTexture texture, float pps, float speed, float gravityComplient, float lifeLength) {
		this->pps = pps;
		this->speed = speed;
		this->gravityComplient = gravityComplient;
		this->lifeLength = lifeLength;
		this->master = master;
		this->texture = texture;
	}

	void generateParticles(glm::vec3 systemCenter, float delta) {
		float particlesToCreate = pps * delta;
		int count = glm::floor(particlesToCreate);
		for (int i = 0; i < count; i++) {
			emitParticle(systemCenter);
		}

	}
private:

	void emitParticle(glm::vec3 center) {
		float dirX = (float)rand() / (float)(RAND_MAX) * 2.0 - 1;
		float dirZ = (float)rand() / (float)(RAND_MAX) * 2.0 - 1;
		glm::vec3 velocity = glm::vec3(dirX, 1, dirZ);
		velocity = glm::normalize(velocity);
		velocity = velocity * speed;	
		master->addParticle(new Particle(texture, glm::vec3(center), velocity, gravityComplient, lifeLength, 0, 1));

	}
};

#endif
