#ifndef H_WATER_TILE
#define H_WATER_TILE

#pragma once

/*
	One body of water is actually water tile. WaterTile has it's position and size.
*/
class WaterTile {
private:
	float height;
	float x, z;

public:
	float TILE_SIZE = 600;
	
	WaterTile(float centerX, float centerZ, float height) {
		this->x = centerX;
		this->z = centerZ;
		this->height = height;
	}

	float getHeight() {
		return height;
	}

	float getX() {
		return x;
	}

	float getZ() {
		return z;
	}

	float getTileSize() {
		return TILE_SIZE;
	}
};

#endif