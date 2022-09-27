#ifndef H_MATHS
#define H_MATHS

#pragma once

/*
	Used to create transformation matrix and berryCentric algorithm (used for terrain generation)
*/
class Maths {
public:

	static float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	static glm::mat4 createTransformationMatrix(glm::vec3 translation, float rx, float ry, float rz, float scale) {
		glm::mat4 matrix = glm::mat4(1.0);
		matrix = glm::translate(matrix, translation);
		
		matrix = glm::rotate(matrix, glm::radians(rx), glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, glm::radians(ry), glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, glm::radians(rz), glm::vec3(0, 0, 1));

		matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

		return matrix;
	}

	static glm::mat4 createViewMatrix(float cameraPitch, float cameraYaw, glm::vec3 cameraPos) {
		glm::mat4 viewMatrix = glm::mat4(1);
	
		viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraPitch), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(cameraYaw), glm::vec3(0, 1, 0));
		// glm::vec3 cameraPos = camera.getPosition();
		glm::vec3 negativeCameraPos = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
		viewMatrix = glm::translate(viewMatrix, negativeCameraPos);

		return viewMatrix;
	}
};


#endif
