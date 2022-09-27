#ifndef H_GUI_TEXTURE
#define H_GUI_TEXTURE

#pragma once
#include <glm/gtc/matrix_transform.hpp>

/*
	WIP - representing GUI texture with position and scale (on screen)
*/
class GuiTexture {
private:
	unsigned int texture;
	glm::vec2 position;
	glm::vec2 scale;

public:

	GuiTexture(unsigned int texture, glm::vec2 position, glm::vec2 scale) {
		this->texture = texture;
		this->position = position;
		this->scale = scale;
	}

	unsigned int getTexture() {
		return texture;
	}

	glm::vec2 getPosition() {
		return position;
	}

	glm::vec2 getScale() {
		return scale;
	}

};

#endif