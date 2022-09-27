#ifndef H_CONTRAST_CHANGER
#define H_CONTRAST_CHANGER


#pragma once
#include "ImageRenderer.h"
#include "ContrastShader.h"

/*
	Post processing effect, it is used inside post processing pipeline.
	Idea of shader: makes darker areas even darker and brighter areas even brighter
*/
class ContrastChanger {

private:
	ImageRenderer* renderer;
	ContrastShader shader;

public:
	ContrastChanger() {
		renderer = new ImageRenderer();
	}

	void render(unsigned int texture) {
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		renderer->renderQuad();
		shader.stop();
	}

	void cleanUp() {
		renderer->cleanUp();
		shader.cleanUp();
	}
};

#endif