#ifndef H_TERRAIN_TEXTURE_PACK
#define H_TERRAIN_TEXTURE_PACK

#pragma once
#include "TerrainTexture.h"

/*
	Each texture pack contains 4 textures: background, r, g and b.
	Blend map is drawn using only 4 colors red, green and blue. Having that in mind each color on blend map
	coresponds with r, g or b texture. If ant other color is used background texture will be used.
*/
class TerrainTexturePack {
private:

	TerrainTexture backgroundTexture;
	TerrainTexture rTexture;
	TerrainTexture gTexture;
	TerrainTexture bTexture;

public:
	TerrainTexturePack(TerrainTexture backgroundTexture, TerrainTexture rTexture,
		TerrainTexture gTexture, TerrainTexture bTexture) {
		this->backgroundTexture = backgroundTexture;
		this->rTexture = rTexture;
		this->gTexture = gTexture;
		this->bTexture = bTexture;
	}
	TerrainTexturePack(){}

	TerrainTexture getBackgroundTexture() {
		return backgroundTexture;
	}

	TerrainTexture getRTexture() {
		return rTexture;
	}

	TerrainTexture getGTexture() {
		return gTexture;
	}

	TerrainTexture getBTexture() {
		return bTexture;
	}

};

#endif