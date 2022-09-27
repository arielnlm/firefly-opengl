#ifndef H_PARTICLE_TEXTURE
#define H_PARTICLE_TEXTURE

#pragma once

/*
	Stores particle's texture and number of rows.
	Number of rows is 1 if it uses normaln texutres.
	Number of rows is 2^n, where n > 1 if particle is using texture atlas.
*/
class ParticleTexture {

private:
	unsigned int textureID;
	int numberOfRows;

public:
	ParticleTexture(unsigned int textureID, int numberOfRows) {
		this->textureID = textureID;
		this->numberOfRows = numberOfRows;
	}

	ParticleTexture() {}

	int getTextureID() {
		return textureID;
	}

	int getNumberOfRows() {
		return numberOfRows;
	}

	void lateInit(unsigned int textureID, int numberOfRows) {
		this->textureID = textureID;
		this->numberOfRows = numberOfRows;
	}

	bool operator<(const ParticleTexture& rhs) const noexcept
	{
		return this->textureID < rhs.textureID;
	}
};

#endif