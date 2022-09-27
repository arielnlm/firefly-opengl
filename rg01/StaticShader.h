#ifndef H_STATICSHADER
#define H_STATICSHADER

#pragma once
#include "Shader.h"
#include "Light.h"
#include "PlayerCamera.h"
#include "Maths.h"
#include <vector>

/*
	Shader that every standard entity uses
*/
class StaticShader : public Shader{

private:
	const char* VERTEX_FILE = "shaders/vertexShader.txt";
	const char* FRAGMENT_FILE = "shaders/fragmentShader.txt";

	int MAX_LIGHTS = 4;

public:
	StaticShader() : Shader("shaders/vertexShader.txt", "shaders/fragmentShader.txt") { }

	void loadShineVariables(float damper, float reflectivity);
	void loadTransformationMatrix(glm::mat4 matrix);
	void loadLights(std::vector<Light> lights);
	void loadLight(Light light);
	void loadViewMatrix(PlayerCamera camera);
	void loadProjectionMatrix(glm::mat4 projectionMatrix);
	void loadFakeLighting(bool useFake);
	void loadSkyColor(float r, float g, float b);
	void loadNumberOfRows(int numberOfRows);
	void loadOffset(float offsetX, float offsetY);
	void loadClipPlane(glm::vec4 plane);

protected:
	void bindAttributes() override;
};

#endif