#include "PlayerCamera.h"

PlayerCamera::PlayerCamera(Player* player) {
	this->player = player;
}

void PlayerCamera::move(Camera_Movement direction) {
	if (direction == Camera_Movement::FORWARD) {
		position.z -= 0.2f;
	}
	if (direction == Camera_Movement::RIGHT) {
		position.x += 0.2f;
	}
	if (direction == Camera_Movement::LEFT) {
		position.x -= 0.2f;
	}
	if (direction == Camera_Movement::BACKWARD) {
		position.z += 0.2f;
	}
	if (direction == Camera_Movement::UP) {
		position.y += 0.2f;
	}
	if (direction == Camera_Movement::DOWN) {
		position.y -= 0.2f;
	}
}

glm::vec3 PlayerCamera::getPosition() {
	return position;
}

float PlayerCamera::getPitch() {
	return pitch;
}

float PlayerCamera::getYaw() {
	return yaw;
}

float PlayerCamera::getRoll() {
	return yaw;
}

void PlayerCamera::calculateZoom(float scrollAmount)
{
	float zoomLevel = scrollAmount;
	distanceFromPlayer -= zoomLevel;
	calculateCameraPosition(calculateHorizontalDistance(), calculateVerticalDistance());
}

void PlayerCamera::calculatePitch(float dy)
{
	float pitchChange = dy * 0.1f;
	pitch -= pitchChange;
	calculateCameraPosition(calculateHorizontalDistance(), calculateVerticalDistance());
}

void PlayerCamera::calculateAngeAroundPlayer(float dx)
{
	float angleChange = dx * 0.3f;
	angleAroundPlayer -= angleChange;
	calculateCameraPosition(calculateHorizontalDistance(), calculateVerticalDistance());
}

void PlayerCamera::updateCameraPosition()
{
	calculateCameraPosition(calculateHorizontalDistance(), calculateVerticalDistance());
}

void PlayerCamera::subtractPosY(float v)
{
	this->position.y -= v;
}

void PlayerCamera::invertPitch()
{
	pitch = -pitch;
}

float PlayerCamera::calculateHorizontalDistance()
{
	return distanceFromPlayer * glm::cos(glm::radians(pitch));
}

float PlayerCamera::calculateVerticalDistance()
{
	return distanceFromPlayer * glm::sin(glm::radians(pitch));
}

void PlayerCamera::calculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	float theta = player->getRotY() + angleAroundPlayer;
	float offsetX = horizontalDistance * glm::sin(glm::radians(theta));
	float offsetZ = horizontalDistance * glm::cos(glm::radians(theta));
	position.x = player->getPosition().x - offsetX;
	position.z = player->getPosition().z - offsetZ;
	position.y = player->getPosition().y + verticalDistance;

	this->yaw = 180 - (player->getRotY() + angleAroundPlayer);
}
