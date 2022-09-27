#ifndef H_SKYBOX_SHADER
#define H_SKYBOX_SHADER

#pragma once
#include "Shader.h"
#include "PlayerCamera.h"
#include "Maths.h"

/*
	Representation of skybox shader
*/
class SkyboxShader : public Shader {
private:
	const float ROTATION_SPEED = 1.0f;
	float rotation = 0;

public:

	SkyboxShader() : Shader("shaders/skyboxVertexShader.txt", "shaders/skyboxFragmentShader.txt") { }

	void loadProjectionMatrix(glm::mat4 matrix) {
		setMat4("projectionMatrix", matrix);
	}

	void loadViewMatrix(PlayerCamera camera, float deltaTime) {
		glm::mat4 viewMatrix = Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition());
		viewMatrix[3][0] = 0;
		viewMatrix[3][1] = 0;
		viewMatrix[3][2] = 0; // ovo da bi se kretao skybox kao i mi
		rotation += ROTATION_SPEED * deltaTime;
		viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
		setMat4("viewMatrix", viewMatrix);
	}

	void loadSkybox() {
		setInt("cubeMap", 0);
	}

	void loadFogColor(float r, float g, float b) {
		setVec3("fogColor", r, g, b);
	}

	void bindAttributes() override {
		Shader::bindAttribute(0, "position");
	}
};

#endif