#ifndef H_HORIZONTAL_BLUR_SHADER
#define H_HORIZONTAL_BLUR_SHADER

#pragma once
#include "Shader.h"

/*
	Shader for horizontal gaussian blur
*/
class HorizontalBlurShader : public Shader {
public:
	HorizontalBlurShader() : Shader("shaders/horizontalBlurVertex.txt", "shaders/blurFragment.txt") {

	}

	void loadTargetWidth(float width) {
		setFloat("targetWidth", width);
	}

	void bindAttributes() {
		bindAttribute(0, "position");
	}
};

#endif