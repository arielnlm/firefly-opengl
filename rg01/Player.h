#ifndef H_PLAYER
#define H_PLAYER

#pragma once
#include "Entity.h"

enum PlayerInput {
	W,A,S,D,NOTHING
};

static const float GRAVITY = -50;

/*
	Player is entity with additional attributes that user is controling (manipulating). 
	It can jump (fly) and move.
*/
class Player: public Entity{

private:
	const float RUN_SPEED = 60;
	const float TURN_SPEED = 160;
	const float JUMP_POWER = 30;

	float upwardsSpeed = 0;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;

	bool isInAir = false;

public:

	Player(TexturedModel* model, glm::vec3 position, float rotX, float rotY, float rotZ, float scale)
		: Entity(model, position, rotX, rotY, rotZ, scale) {
	}

	void move(PlayerInput input, float terrainHeight, float frameTimeSeconds, bool alreadyCalculated) {
		checkInputs(input);
		increaseRotation(0, currentTurnSpeed * frameTimeSeconds, 0);
		float distance = currentSpeed * frameTimeSeconds;
		float dx = distance * glm::sin(glm::radians(getRotY()));
		float dz = distance * glm::cos(glm::radians(getRotY()));
		increasePosition(dx, 0, dz);
		if (alreadyCalculated) {
			return;
		}
		upwardsSpeed += GRAVITY * frameTimeSeconds;
		increasePosition(0, upwardsSpeed * frameTimeSeconds, 0);
		
		if (getPosition().y < terrainHeight) {
			upwardsSpeed = 0;
			setPositionY(terrainHeight);
			isInAir = false;
		}
	}

	void jump() {
		//if (!isInAir) { // if you want normal jumping remove comments
			upwardsSpeed = JUMP_POWER;
			isInAir = true;
		//}
	}

private:
	void checkInputs(PlayerInput input) {
		if (input == PlayerInput::W) {
			currentSpeed = RUN_SPEED;
		}
		else if (input == PlayerInput::S) {
			currentSpeed = -RUN_SPEED;
		}
		else {
			currentSpeed = 0;
		}

		if (input == PlayerInput::D) {
			currentTurnSpeed = -TURN_SPEED;
		}
		else if (input == PlayerInput::A) {
			currentTurnSpeed = TURN_SPEED;
		}
		else {
			currentTurnSpeed = 0;
		}
	}

};
#endif