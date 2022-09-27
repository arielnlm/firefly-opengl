#ifndef H_PARTICLE_SHADER
#define H_PARTICLE_SHADER

#pragma once
#include "Shader.h"

/*
	Represents particle's shaders.
*/
class ParticleShader : public Shader{
public:
	ParticleShader() : Shader("shaders/particleVShader.txt", "shaders/particleFShader.txt") {

	}

	void bindAttributes() {
		bindAttribute(0, "position");
	}

	void loadModelViewMatrix(glm::mat4 modelView) {
		setMat4("modelViewMatrix", modelView);
	}

	void loadProjectionMatrix(glm::mat4 projectionMatrix) {
		setMat4("projectionMatrix", projectionMatrix);
	}

	void loadTextureCoordInfo(glm::vec2 offset1, glm::vec2 offset2, float numRows, float blend) {
		setVec2("texOffset1", offset1);
		setVec2("texOffset2", offset2);
		setVec2("texCoordInfo", numRows, blend);
	}
};

#endif