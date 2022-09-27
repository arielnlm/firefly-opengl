#ifndef H_IMAGE_RENDERER
#define H_IMAGE_RENDERER

#pragma once
#include "Fbo.h"

/*
	Binds frame buffer and renders (draws) quad on screen
*/
class ImageRenderer {
private:
	Fbo* fbo;

public:
	ImageRenderer(int width, int height) {
		fbo = new Fbo(width, height, 0);
	}

	ImageRenderer() {}

	void renderQuad() {
		if (fbo) {
			fbo->bindFrameBuffer();
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		if (fbo) {
			fbo->unbindFrameBuffer();
		}
	}

	unsigned int getOutputTexture() {
		return fbo->getColourTexture();
	}

	void cleanUp() {
		if (fbo) {
			fbo->cleanUp();
		}
	}
}; 

#endif 