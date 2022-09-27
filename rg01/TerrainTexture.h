#ifndef H_TERRAIN_TEXTURE
#define H_TERRAIN_TEXTURE

#pragma once

/*
	Stores terrain's texture ID
*/

class TerrainTexture {
private:
	int textureID;

public:
	TerrainTexture(int textureID) {
		this->textureID = textureID;
	}

	TerrainTexture(){}

	int getTextureID() {
		return textureID;
	}
};

#endif