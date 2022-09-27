#ifndef H_SKYBOX_RENDERER
#define H_SKYBOX_RENDERER

#pragma once

#include <string>
#include "RawModel.h"
#include "SkyboxShader.h"
#include "Loader.h"
/*
	renders skybox using cubemaps
*/
class SkyboxRenderer {
private:
	float SIZE = 15000.0f; // change to make skybox bigger or smaller

	float VERTICES[108] = {
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
	};

	std::string TEXTURE_FILES[6] = {
		"resources/textures/skybox/nightRight.png",
		"resources/textures/skybox/nightLeft.png",
		"resources/textures/skybox/nightTop.png",
		"resources/textures/skybox/nightBottom.png",
		"resources/textures/skybox/nightFront.png",
		"resources/textures/skybox/nightBack.png",
	};

	RawModel* cube;
	unsigned int texture;
	SkyboxShader shader;

public:
	SkyboxRenderer(Loader* loader, glm::mat4 projectionMatrix) {
		cube = loader->loadToVAO(VERTICES, 108*sizeof(float), 3);
		texture = loader->loadCubeMap(TEXTURE_FILES, 6);
		shader.use();
		shader.loadSkybox();
		shader.loadProjectionMatrix(projectionMatrix);
		shader.stop();
	}

	void render(PlayerCamera camera, float r, float g, float b, float deltaTime) {
		glDepthFunc(GL_LEQUAL);
		shader.use();
		shader.loadViewMatrix(camera, deltaTime);
		shader.loadFogColor(r, g, b);
		glBindVertexArray(cube->getVaoID());
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, cube->getVertexCount());
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		shader.stop();
	}
};

#endif
