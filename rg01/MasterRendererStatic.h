#ifndef H_MASTER_RENDERER_STATIC
#define H_MASTER_RENDERER_STATIC

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

/*
	Helper static culling functions
*/
class MasterRendererStatic {
public:
	static void enableCulling() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	static void disableCulling() {
		glDisable(GL_CULL_FACE);
	}
};

#endif