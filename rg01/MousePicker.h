#ifndef H_MOUSE_PICKER
#define H_MOUSE_PICKER

#pragma once
#include "Maths.h"
#include <glm/gtc/matrix_transform.hpp>

/*
	WIP: ViewPort space -> normalized device space -> homogeneous clip space 
	-> eye space -> world space -> local space
*/

class MousePicker {

private:
	const int RECURSION_COUNT = 200;
	const float RAY_RANGE = 600;

	glm::vec3 currentRay;
	
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	PlayerCamera camera;

	float SCREEN_WIDTH = 1280;
	float SCREEN_HEIGHT = 720;
	float cameraPitch, cameraYaw;
	glm::vec3 cameraPos;

	Terrain terrain;
	glm::vec3 currentTerrainPoint;

public:
	MousePicker(PlayerCamera cam, glm::mat4 projection) {
		this->camera = cam;
		projectionMatrix = projection;
		viewMatrix = Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition());
	}

	glm::vec3 getCurrentRay() {
		return currentRay;
	}

	void update(float mouseX, float mouseY, PlayerCamera camera) {
		cout << "VIEW MATRIX " << mouseX << " " << mouseY << "\n";
		viewMatrix = Maths::createViewMatrix(camera.getPitch(), camera.getYaw(), camera.getPosition());
		currentRay = calculateMouseRay(mouseX, mouseY);
	}
private:
	glm::vec3 calculateMouseRay(float mouseX, float mouseY) {
		glm::vec2 normalizedCoords = getNormalizedDeviceCoords(mouseX, mouseY);
		glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, -1, 1);
		glm::vec4 eyeCoords = toEyeCoords(clipCoords);
		glm::vec3 worldRay = toWorldCoords(eyeCoords);

		return worldRay;
	}

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords) {
		glm::mat4 invertedView = glm::inverse(viewMatrix);
		glm::vec4 rayWorld = invertedView * eyeCoords;
		glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
		mouseRay = glm::normalize(mouseRay);

		return mouseRay;
	}

	glm::vec2 getNormalizedDeviceCoords(float mouseX, float mouseY) {
		float x = (2.0 * mouseX) - SCREEN_WIDTH - 1;
		// float y = (2.0 * mouseY) / SCREEN_HEIGHT - 1;
		float y = (mouseY + SCREEN_HEIGHT / 2) / SCREEN_HEIGHT * 2.f;
		return glm::vec2(x, -y); // mozda gresi
	}

	glm::vec4 toEyeCoords(glm::vec4 clipCoords) {
		glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
		glm::vec4 eyeCoords = invertedProjection * clipCoords;
		return glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);
	}
};

#endif