#include "Entity.h"

Entity::Entity(TexturedModel* model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale) {
	this->model = model;
	this->position = position;
	this->rotX = rotX;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->scale = scale;
}

Entity::Entity(TexturedModel* model, int index, glm::vec3 position, float rotX, float rotY, float rotZ, float scale) {
	this->model = model;
	this->textureIndex = index;
	this->position = position;
	this->rotX = rotX;
	this->rotY = rotY;
	this->rotZ = rotZ;
	this->scale = scale;
}

void Entity::increasePosition(float dx, float dy, float dz) {
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;
}

void Entity::increaseRotation(float dx, float dy, float dz) {
	this->rotX += dx;
	this->rotY += dy;
	this->rotZ += dz;
}

float Entity::getTextureXOffset()
{
	int column = textureIndex % model->getTexture()->getNumberOfRows();
	return 1.0f * column / model->getTexture()->getNumberOfRows();
}

float Entity::getTextureYOffset()
{
	int row = textureIndex / model->getTexture()->getNumberOfRows();
	return 1.0f * row / model->getTexture()->getNumberOfRows();
}

TexturedModel* Entity::getModel() {
	return this->model;
}

glm::vec3 Entity::getPosition() {
	return this->position;
}

float Entity::getRotX() {
	return rotX;
}

float Entity::getRotY() {
	return rotY;
}

float Entity::getRotZ() {
	return rotZ;
}

float Entity::getScale() {
	return scale;
}

void Entity::setPositionY(float y) {
	this->position.y = y;
}