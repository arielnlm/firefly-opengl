#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color) {
	this->position = position;
	this->color = color;
}

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation)
{
	this->position = position;
	this->color = color;
	this->attenuation = attenuation;
}

glm::vec3 Light::getPosition() {
	return position;
}

glm::vec3 Light::getColor() {
	return color;
}

glm::vec3 Light::getAttenuation()
{
	return attenuation;
}

void Light::setAttenuation(glm::vec3 a)
{
	this->attenuation = a;
}

void Light::setPosition(glm::vec3 pos) {
	position = pos;
}

void Light::setColor(glm::vec3 col) {
	color = color;
}