#ifndef H_POST_PROCESSING
#define H_POST_PROCESSING

#pragma once
#include "RawModel.h"
#include <glad/glad.h>
#include "ContrastChanger.h"
#include "Loader.h"
#include "HorizontalBlur.h"
#include "VerticalBlur.h"

/*
	Represent post processing pipeline, can easily combine multiple post processing effects.
	Applies desired post processing effects on screen.
*/
class PostProcessing {
private:
	float SCREEN_WIDTH = 1280.0;
	float SCREEN_HEIGHT = 720.0;
	RawModel* quad;
	ContrastChanger* contrastChanger;
	HorizontalBlur* hBlur;
	VerticalBlur* vBlur;

	float* POSITIONS = new float[8]{
		-1, 1, -1, -1, 1, 1, 1, -1
	};

public:

	PostProcessing(Loader* loader) {
		quad = loader->loadToVAO(POSITIONS, 8 * sizeof(float), 2);
		contrastChanger = new ContrastChanger();
		hBlur = new HorizontalBlur(SCREEN_WIDTH, SCREEN_HEIGHT);
		vBlur = new VerticalBlur(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	void doPostProcessing(unsigned int colorTexture, bool shouldBlurScreen) {
		start();
		if (!shouldBlurScreen) {
			contrastChanger->render(colorTexture);
		}
		else {
			hBlur->render(colorTexture);
			vBlur->render(hBlur->getOutputTexture());
			contrastChanger->render(vBlur->getOutputTexture());
		}

		end();
	}

	void cleanUp() {
		contrastChanger->cleanUp();
		hBlur->cleanUp();
		vBlur->cleanUp();
	}

private:

	void start() {
		glBindVertexArray(quad->getVaoID());
		glEnableVertexAttribArray(0);
		glDisable(GL_DEPTH_TEST);
	}

	void end() {
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};

#endif