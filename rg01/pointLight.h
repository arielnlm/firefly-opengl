#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

#include <vector>
class PointLight {
	public:
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;

		void PopulateShader(Shader lightingShader, unsigned int index) {
			char buffer[64];
			sprintf(buffer, "pointLights[%i].position", index);
			lightingShader.setVec3(buffer, position);
			sprintf(buffer, "pointLights[%i].ambient", index);
			lightingShader.setVec3(buffer, ambient);
			sprintf(buffer, "pointLights[%i].diffuse", index);
			lightingShader.setVec3(buffer, diffuse);
			sprintf(buffer, "pointLights[%i].specular", index);
			lightingShader.setVec3(buffer, specular);
			sprintf(buffer, "pointLights[%i].constant", index);
			lightingShader.setFloat(buffer, constant);
			sprintf(buffer, "pointLights[%i].linear", index);
			lightingShader.setFloat(buffer, linear);
			sprintf(buffer, "pointLights[%i].quadratic", index);
			lightingShader.setFloat(buffer, quadratic);
		}
};