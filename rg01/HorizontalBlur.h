#ifndef H_HORIZONTAL_BLUR
#define H_HORIZONTAL_BLUR

#pragma once
#include "ImageRenderer.h"
#include "HorizontalBlurShader.h"

/*
	Post processing effect that usese imageRenderer and it's shader (HorizontalBlurShader) 
	to apply horizontal gaussian blur. Effect is stored in separate frame buffer object (fbo) 
	which is later drawn as final output that user sees.
*/
class HorizontalBlur {
private:
	ImageRenderer* renderer;
	HorizontalBlurShader shader;

public:
	HorizontalBlur(int targetFboWidth, int targetFboHeight) {
		shader.use();
		shader.loadTargetWidth(targetFboWidth);
		shader.stop();
		renderer = new ImageRenderer(targetFboWidth, targetFboHeight);
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