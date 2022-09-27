#ifndef H_PLAYERCAMERA
#define H_PLAYERCAMERA

#pragma once
#include "Player.h"

class PlayerCamera {
public:

	static enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		DOWN,
		UP
	};

private:

	float distanceFromPlayer = 150;
	float angleAroundPlayer = 0;

	glm::vec3 position = glm::vec3(0, 5, 0);
	float pitch = 20;
	float yaw;
	float roll;

	Player* player;

public:
	PlayerCamera(Player* player);
	PlayerCamera(){}
	void move(Camera_Movement direction);

	glm::vec3 getPosition();

	float getPitch();
	float getYaw();
	float getRoll();

	void calculateZoom(float scrollAmount);//scroll wheel
	void calculatePitch(float dy);//rightclick i gore dole pomeraj
	void calculateAngeAroundPlayer(float dx);//leftclick i levo desno pomeraj
	Player* getPlayer() { return player; }
	void setPlayer(Player* player) { this->player = player; }

	void updateCameraPosition();
	void subtractPosY(float v);
	void invertPitch();

private:
	float calculateHorizontalDistance();
	float calculateVerticalDistance();
	void calculateCameraPosition(float horizontalDistance, float verticalDistance);
};
#endif