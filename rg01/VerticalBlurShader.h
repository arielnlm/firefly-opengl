#ifndef H_VERTICAL_BLUR_SHADER
#define H_VERTICAL_BLUR_SHADER

#pragma once
#include "Shader.h"

/*
	Shader for horizontal gaussian blur
*/
class VerticalBlurShader : public Shader {
public:
	VerticalBlurShader() : Shader("shaders/verticalBlurVertex.txt", "shaders/blurFragment.txt") {

	}

	void loadTargetHeight(float height) {
		setFloat("targetHeight", height);
	}

	void bindAttributes() {
		bindAttribute(0, "position");
	}
};

#endif