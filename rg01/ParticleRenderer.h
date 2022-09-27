#ifndef H_PARTICLE_RENDERER
#define H_PARTICLE_RENDERER

#pragma once
#include "ParticleShader.h"
#include "RawModel.h"
#include "Loader.h"
#include "Particle.h"
#include "PlayerCamera.h"
#include "Maths.h"

/*
	Prepares and updates particle's position, rotation etc.,  if necessery.
*/
class ParticleRenderer {
private:

	float* VERTICES = new float[8]{ -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };

	RawModel* quad;
	ParticleShader shader;

public:
	ParticleRenderer(Loader* loader, glm::mat4 projectionMatrix) {
		quad = loader->loadToVAO(VERTICES, 8 * sizeof(float), 2);
		shader.use();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
	}

	void render(map<ParticleTexture, vector<Particle*>> particles, PlayerCamera camera) {
		glm::mat4 viewMatrix = Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition());
		prepare();
		for (auto entry : particles) {
			// bind texture
			ParticleTexture texture = entry.first;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
			for (Particle* p : entry.second) {
				updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), viewMatrix);
				shader.loadTextureCoordInfo(p->getTexOffset1(), p->getTexOffset2(), texture.getNumberOfRows(), p->getBlend());
				glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
			}
		}	
		finishRendering();
	}

	void cleanUp() {
		shader.cleanUp();
	}

private:

	void prepare() {
		shader.use();
		glBindVertexArray(quad->getVaoID());
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(false);
	}

	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix) {
		glm::mat4 modelMatrix = glm::mat4(1.0);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix[0][0] = viewMatrix[0][0];
		modelMatrix[0][1] = viewMatrix[1][0];
		modelMatrix[0][2] = viewMatrix[2][0];
		modelMatrix[1][0] = viewMatrix[0][1];
		modelMatrix[1][1] = viewMatrix[1][1];
		modelMatrix[1][2] = viewMatrix[2][1];
		modelMatrix[2][0] = viewMatrix[0][2];
		modelMatrix[2][1] = viewMatrix[1][2];
		modelMatrix[2][2] = viewMatrix[2][2];
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));

		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		shader.loadModelViewMatrix(modelViewMatrix);
	}

	void finishRendering() {
		glDepthMask(true);
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();
	}
};

#endif