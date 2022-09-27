#ifndef H_PARTICLE
#define H_PARTICLE

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ParticleTexture.h"

static int cnt = 0;

/*
	Represent one particle on screen. Particle is made of it's texture, position, rotation,
	scale, velocity, life span
*/
class Particle {

private:
	glm::vec3 position;
	glm::vec3 velocity;
	float gravityEffect;
	float lifeLength;
	float rotation;
	float scale;

	float elapsedTime = 0;
	float GRAVITY = -50;

	ParticleTexture texture;

	glm::vec2 texOffset1;
	glm::vec2 texOffset2;
	float blend;
	float distance;

public:
	int myId = 0;
	Particle(ParticleTexture texture, glm::vec3 position, glm::vec3 velocity, float gravityEffect,
		float lifeLength, float rotation, float scale) {
		this->position = position;
		this->velocity = velocity;
		this->gravityEffect = gravityEffect;
		this->lifeLength = lifeLength;
		this->rotation = rotation;
		this->scale = scale;
		this->texture = texture;
		myId = cnt++;
	}

	bool update(float deltaTime, PlayerCamera camera) {
		velocity.y += GRAVITY * gravityEffect * deltaTime;
		glm::vec3 change = glm::vec3(velocity);
		change = change * deltaTime; // trena SCALE
		position = change + position;
		distance = glm::length(camera.getPosition() - position);
		distance *= distance;

		updateTextureCoordInfo();
		elapsedTime += deltaTime;
		return elapsedTime < lifeLength;
	}

	float getDistance() {
		return distance;
	}

	glm::vec3 getPosition() {
		return position;
	}
	
	float getRotation() {
		return rotation;
	}

	float getScale() {
		return scale;
	}

	ParticleTexture getTexture() {
		return texture;
	}

	glm::vec2 getTexOffset1() {
		return texOffset1;
	}

	glm::vec2 getTexOffset2() {
		return texOffset2;
	}

	float getBlend() {
		return blend;
	}

private:
	void updateTextureCoordInfo() {
		float lifeFactor = elapsedTime / lifeLength;
		int stageCount = texture.getNumberOfRows() * texture.getNumberOfRows();
		float atlasProgression = lifeFactor * stageCount;
		int index1 = glm::floor(atlasProgression);
		int index2 = index1 < stageCount - 1 ? index1 + 1 : index1;
		this->blend = atlasProgression - index1;
		setTextureOffset(texOffset1, index1);
		setTextureOffset(texOffset2, index2);
	}

	void setTextureOffset(glm::vec2 &offset, int index) {
		int column = index % texture.getNumberOfRows();
		int row = index / texture.getNumberOfRows();
		offset.x = column * 1.0 / texture.getNumberOfRows();
		offset.y = row * 1.0 / texture.getNumberOfRows();
	}
};

#endif