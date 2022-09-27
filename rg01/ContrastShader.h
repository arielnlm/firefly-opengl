#ifndef H_CONTRAST_SHADER
#define H_CONTRAST_SHADER

#pragma once
#include "Shader.h"

/*
	Class representation of two contrast shaders
*/
class ContrastShader : public Shader {

public:
	ContrastShader() : Shader("shaders/contrastVertex.txt", "shaders/contrastFragment.txt") {

	}

	void bindAttributes() {
		bindAttribute(0, "position");
	}
};

#endif