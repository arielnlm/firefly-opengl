#ifndef H_TERRAINSHADER
#define H_TERRAINSHADER

#pragma once
#include "Shader.h"
#include "Light.h"
#include "PlayerCamera.h"
#include "Maths.h"
#include <vector>

/*
	Representation of terrain's vertex and fragment shader
*/
class TerrainShader : public Shader {

private:
	const char* VERTEX_FILE = "shaders/terrainVertexShader.txt";
	const char* FRAGMENT_FILE = "shaders/terrainFragShader.txt";

	int MAX_LIGHTS = 4;

public:
	TerrainShader() : Shader("shaders/terrainVertexShader.txt", "shaders/terrainFragShader.txt") {}

	void loadShineVariables(float damper, float reflectivity);
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadLight(Light light);
	void loadViewMatrix(PlayerCamera camera);
	void loadProjectionMatrix(glm::mat4 projectionMatrix);
	void loadSkyColor(float r, float g, float b);
	void connectTextureUnits();
	void loadLights(std::vector<Light> lights);
	void loadClipPlane(glm::vec4 plane);

protected:
	void bindAttributes() override;
};
#endif