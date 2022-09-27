#ifndef H_GUI_RENDERER
#define H_GUI_RENDERER

#pragma once
#include "Loader.h"
#include "GuiTexture.h"

/*
	WIP - used to render GUI on screen (quad)
*/
class GUIRenderer {

private:
	RawModel* quad;

public:
	GUIRenderer(Loader* loader) {
		float* positions = new float[] {
			-1, 1, -1, -1, 1, 1, 1, -1
		};
		quad = loader->loadToVAO(positions, 8 * sizeof(float), 2);
	}

	void render(vector<GuiTexture> guis) {
		glBindVertexArray(quad->getVaoID());
		glEnableVertexAttribArray(0);
		for (GuiTexture gui : guis) {
			glDrawArrays(GL_TRIANGLE_STRIP, 0, quad->getVertexCount());
		}
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};

#endif