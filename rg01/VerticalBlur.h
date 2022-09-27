#ifndef H_VERTICAL_BLUR
#define H_VERTICAL_BLUR

#pragma once
#include "ImageRenderer.h"
#include "VerticalBlurShader.h"

/*
	Post processing effect that usese imageRenderer and it's shader (VerticalBlurShader)
	to apply vertical gaussian blur. Effect is stored in separate frame buffer object (fbo)
	which is later drawn as final output that user sees.
*/
class VerticalBlur {
private:
	ImageRenderer* renderer;
	VerticalBlurShader shader;

public:
	VerticalBlur(int targetFboWidth, int targetFboHeight) {
		renderer = new ImageRenderer(targetFboWidth, targetFboHeight);
		shader.use();
		shader.loadTargetHeight(targetFboHeight);
		shader.stop();
	}

public:
	void render(unsigned int texture) {
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		renderer->renderQuad();
		shader.stop();
	}

	int getOutputTexture() {
		return renderer->getOutputTexture();
	}

	void cleanUp() {
		renderer->cleanUp();
		shader.cleanUp();
	}
};

#endif