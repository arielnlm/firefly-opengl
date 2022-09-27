#ifndef H_WATER_RENDERER
#define H_WATER_RENDERER

#pragma once

#include "WaterShader.h"
#include "Loader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"

/*
	Prepares and renders water using dudv and normal map;
	dudv - adds water randomness 
	normalMap - simulates water normals, that makes water look like it's not flat and static
*/
class WaterRenderer {
private:

	float WAVE_SPEED = 0.03f;
	float moveFactor = 0;

	const char* DUDV_MAP = "resources/textures/waterDUDV.png";
	const char* NORMAL_MAP = "resources/textures/matchingNormalMap.png";
	RawModel* quad;
	WaterShader shader;
	WaterFrameBuffers* fbos;

	unsigned int dudvTexture;
	unsigned int normalMap;

public:
	WaterRenderer(Loader* loader, WaterShader shader, glm::mat4 projectionMatrix, WaterFrameBuffers* fbos) {
		this->shader = shader;
		this->fbos = fbos;
		dudvTexture = loader->loadTexture(DUDV_MAP);
		normalMap = loader->loadTexture(NORMAL_MAP);
		shader.use();
		shader.connectTextureUnits();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
		setUpVAO(loader);
	}

	void render(vector<WaterTile> water, PlayerCamera camera, float delta, Light sun) {
		prepareRander(camera, delta, sun);
		for (WaterTile tile : water) {
			glm::mat4 modelMatrix = Maths::createTransformationMatrix(
				glm::vec3(tile.getX(), tile.getHeight(), tile.getZ()),
				0, 0, 0, tile.getTileSize()
			);
			shader.loadModelMatrix(modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, quad->getVertexCount());
		}
		unbind();
	}

	void prepareRander(PlayerCamera camera, float delta, Light sun) {
		shader.use();
		shader.loadViewMatrix(camera);
		moveFactor += WAVE_SPEED * delta;
		if (moveFactor >= 1)
			moveFactor = 0;
		shader.loadLight(sun);
		shader.loadMoveFactor(moveFactor);
		glBindVertexArray(quad->getVaoID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTexture());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudvTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, fbos->getRefractionDepthTexture());

	}

	void unbind() {
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		shader.stop();
	}

	void setUpVAO(Loader* loader) {
		float vertices[12] = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };
		quad = loader->loadToVAO(vertices, 12*sizeof(float), 2);
	}
};

#endif