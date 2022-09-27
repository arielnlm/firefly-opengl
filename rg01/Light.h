#ifndef H_LIGHT
#define H_LIGHT

#pragma once
#include <glm/gtc/matrix_transform.hpp>

/*
	Stores basic light information: position, light color, attenuation (if point light)
*/
class Light {
private:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation = glm::vec3(1, 0, 0);


public:
	Light(glm::vec3 position, glm::vec3 color);
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation);

	glm::vec3 getPosition();
	glm::vec3 getColor();
	glm::vec3 getAttenuation();

	void setAttenuation(glm::vec3 a);
	void setPosition(glm::vec3 pos);
	void setColor(glm::vec3 col);
};
#endif