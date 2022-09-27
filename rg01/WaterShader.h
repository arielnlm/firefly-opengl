#ifndef H_WATER_SHADER
#define H_WATER_SHADER

#pragma once
#include "Shader.h"
#include "PlayerCamera.h"
#include "Maths.h"

/*
	Represents water shaders
*/
class WaterShader : public Shader {

public:
	WaterShader() : Shader("shaders/waterVertex.txt", "shaders/waterFragment.txt") {

	}

	void bindAttributes() {
		bindAttribute(0, "position");
	}

	void loadViewMatrix(PlayerCamera camera) {
		setMat4("viewMatrix", Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition()));
	}

	void loadProjectionMatrix(glm::mat4 projectionMatrix) {
		setMat4("projectionMatrix", projectionMatrix);
	}

	void loadModelMatrix(glm::mat4 modelMatrix) {
		setMat4("modelMatrix", modelMatrix);
	}

	void connectTextureUnits() {
		setInt("reflectionTexture", 0);
		setInt("refractionTexture", 1);
		setInt("dudvMap", 2);
		setInt("normalMap", 3);
		setInt("depthMap", 4);
	}

	void loadMoveFactor(float factor) {
		setFloat("moveFactor", factor);
	}

	void loadCameraPosition(glm::vec3 pos) {
		setVec3("cameraPosition", pos);
	}

	void loadLight(Light sun) {
		setVec3("lightPosition", sun.getPosition());
		setVec3("lightColor", sun.getColor());
	}
};

#endif