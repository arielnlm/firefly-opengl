#ifndef H_MASTERRENDERER
#define H_MASTERRENDERER

#pragma once
#include "StaticShader.h"
#include "EntityRenderer.h"
#include "TexturedModel.h"
#include "Entity.h"
#include <map>
#include <vector>
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "SkyboxRenderer.h"

using namespace std;

/*
	Main game renderer which renders everything on screen.
*/
class MasterRenderer {
private:

	const float FOV = 70.0f;
	const float NEAR_PLANE = 0.1f;
	const float FAR_PLANE = 1000.0f;
	const float SCREEN_WIDTH = 1280.0f;
	const float SCREEN_HEIGHT = 720.0f;

	const float RED = 5.0/256;
	const float GREEN = 14.0/256;
	const float BLUE = 8.0/256;

	glm::mat4 projectionMatrix;

	StaticShader shader;
	EntityRenderer* renderer;

	TerrainRenderer* terrainRenderer;
	TerrainShader terrainShader;

	map<TexturedModel*, vector<Entity*>> entities;
	vector<Terrain> terrains;

	SkyboxRenderer* skyboxRenderer;

public:

	MasterRenderer(Loader* loader) {
		enableCulling();
		createProjectionMatrix();
		renderer = new EntityRenderer(shader, projectionMatrix);
		terrainRenderer = new TerrainRenderer(terrainShader, projectionMatrix);
		skyboxRenderer = new SkyboxRenderer(loader, projectionMatrix);
	}

	void renderScene(vector<Entity*> entities, Terrain terrain, vector<Light> lights, PlayerCamera camera, float deltaTime, glm::vec4 clipPlane) {
		
		processTerrain(terrain);
		for (Entity* e : entities) {
			processEntity(e);
		}
		render(lights, camera, deltaTime, clipPlane);
	}

	static void enableCulling() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	static void disableCulling() {
		glDisable(GL_CULL_FACE);
	}

	void prepare() {
		glEnable(GL_DEPTH_TEST);
		glClearColor(RED, GREEN, BLUE, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void render(std::vector<Light> lights, PlayerCamera camera, float deltaTime, glm::vec4 clipPlane) {
		prepare();
		shader.use();
		shader.loadClipPlane(clipPlane);
		shader.loadSkyColor(RED, GREEN, BLUE);
		shader.loadLights(lights);
		shader.loadViewMatrix(camera);
		renderer->render(entities);
		shader.stop();

		terrainShader.use();
		terrainShader.loadClipPlane(clipPlane);
		terrainShader.loadSkyColor(RED, GREEN, BLUE);
		terrainShader.loadLights(lights);
		terrainShader.loadViewMatrix(camera);
		terrainRenderer->render(terrains);
		terrainShader.stop();

		skyboxRenderer->render(camera, RED, GREEN, BLUE, deltaTime);

		terrains.clear();
		entities.clear();
	}

	void processTerrain(Terrain terrain) {
		terrains.push_back(terrain);
	}

	void processEntity(Entity* entity) {
		TexturedModel* entityModel = entity->getModel();
		if (entities[entityModel].size() != 0) {
			entities[entityModel].push_back(entity);
		}
		else {
			const std::vector<Entity*> batch = { entity };
			entities[entityModel] = batch;
		}
	}

	glm::mat4 getProjectionMatrix() {
		return projectionMatrix;
	}

	void cleanUp() {
		shader.cleanUp();
		terrainShader.cleanUp();
	}

private:

	void createProjectionMatrix() {
		float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
		float y_scale = (1.f / glm::tan(glm::radians(FOV / 2.f))) * aspectRatio;
		float x_scale = y_scale / aspectRatio;
		float frutsum_length = FAR_PLANE - NEAR_PLANE;

		projectionMatrix = glm::mat4(1);
		projectionMatrix[0][0] = x_scale;
		projectionMatrix[1][1] = y_scale;
		projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) - frutsum_length);
		projectionMatrix[2][3] = -1;
		projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frutsum_length);
		projectionMatrix[3][3] = 0;
	}
};
#endif