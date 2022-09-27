#ifndef H_ENTITY
#define H_ENTITY

#pragma once

#include "TexturedModel.h"
#include <glm/gtc/matrix_transform.hpp>

/*
	In world 'object' aka entity: TexturedModel combined with position, rotation and scale
*/
class Entity {
private:
	TexturedModel* model;

	glm::vec3 position;

	float rotX, rotY, rotZ;
	float scale;

	int textureIndex = 0;

public:
	Entity(TexturedModel* model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);
	Entity(TexturedModel* model, int index, glm::vec3 position, float rotX, float rotY, float rotZ, float scale);

	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	
	float getTextureXOffset();
	float getTextureYOffset();
	
	TexturedModel* getModel();
	glm::vec3 getPosition();
	float getRotX();
	float getRotY();
	float getRotZ();
	float getScale();
	void setPositionY(float y);
};
#endif