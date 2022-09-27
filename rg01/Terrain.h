#ifndef H_TERRAIN
#define H_TERRAIN

#pragma once
#include "RawModel.h"
#include "ModelTexture.h"
#include "Loader.h"
#include "TerrainTexturePack.h"
#include "TerrainTexture.h"
#include "Maths.h"
#include "HeightsGenerator.h"

/*
	Terrain model represented with it's texture pack and blend map.
	Texture pack - stores multiple textures which terrain uses to achieve different ground looks
	BlendMap - where each texture, from texture pack, should be drawn
*/
class Terrain {

private:
	const float SIZE = 800.0f;
	const float MAX_HEIGHT = 40;
	const float MAX_PIXEL_COLOR = 256 * 256 * 256;

	float x, z;
	RawModel* model;
	TerrainTexturePack texturePack;
	TerrainTexture blendMap;

	vector<vector<float>> heights;

public:
	Terrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack texturePack, TerrainTexture blendMap, const char* hightmapFile) {
		this->texturePack = texturePack;
		this->blendMap = blendMap;
		this->x = gridX * SIZE;
		this->z = gridZ * SIZE;
		this->model = generateTerrain(loader, hightmapFile);
	}
	Terrain() {
	}
	void lateLoadTerrain(int gridX, int gridZ, Loader* loader, TerrainTexturePack texturePack, TerrainTexture blendMap, const char* hightmapFile) {
		this->texturePack = texturePack;
		this->blendMap = blendMap;
		this->x = gridX * SIZE;
		this->z = gridZ * SIZE;
		this->model = generateTerrain(loader, hightmapFile);
	}
private:
	RawModel* generateTerrain(Loader* loader, const char* heightMap) {

		HeightGenerator* generator = new HeightGenerator();

		int width, height, nrComponents;
		unsigned char* image = stbi_load(heightMap, &width, &height, &nrComponents, 1);

		int VERTEX_COUNT = 128;
		//resizing
		heights.resize(VERTEX_COUNT, vector<float>(VERTEX_COUNT, 0));

		int count = VERTEX_COUNT * VERTEX_COUNT;
		unsigned int vertNormCount = count * 3, texCount = count * 2, 
			indicesCount = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);
		float* vertices = new float[vertNormCount];
		float* normals = new float[vertNormCount];
		float* textureCoords = new float[texCount];
		unsigned int* indices = new unsigned int[indicesCount];
		int vertexPointer = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				vertices[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
				float hgt = getHeight(j, i, generator);
				vertices[vertexPointer * 3 + 1] = hgt;
				heights[j][i] = hgt;
				vertices[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
				glm::vec3 normal = calculateNormal(j, i, generator);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				textureCoords[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);
				textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);
				vertexPointer++;
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < VERTEX_COUNT - 1; gz++) {
			for (int gx = 0; gx < VERTEX_COUNT - 1; gx++) {
				int topLeft = (gz * VERTEX_COUNT) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
		return loader->loadToVAO(vertices, vertNormCount * sizeof(float), indices, indicesCount * sizeof(unsigned int),
			textureCoords, texCount * sizeof(float), normals, vertNormCount * sizeof(float));
	}

	float getHeight(int x, int z, HeightGenerator* generator) {
		return generator->generateHeight(x, z);
	}

	glm::vec3 calculateNormal(int x, int y, HeightGenerator* generator) {
		float heightL = getHeight(x - 1, y, generator);
		float heightR = getHeight(x + 1, y, generator);
		float heightD = getHeight(x , y - 1, generator);
		float heightU = getHeight(x - 1, y - 1, generator);
		glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
		normal = glm::normalize(normal);
		return normal;
	}

public:

	RawModel* getModel() {
		return model;
	}

	TerrainTexturePack getTexturePack() {
		return texturePack;
	}

	TerrainTexture getBlendMap() {
		return blendMap;
	}

	float getX() {
		return x;
	}

	float getZ() {
		return z;
	}

	float getHieghtOfTerrain(float worldX, float worldZ) {
		float terrainX = worldX - this->x;
		float terrainZ = worldZ - this->z;
		float gridSquareSize = SIZE * 1.0 / (heights.size() - 1);
		int gridX = glm::floor(terrainX / gridSquareSize); // dobijamo koordinate kao su 13.2 i 2.1 i to su zapravo 13,2
		int gridZ = glm::floor(terrainZ / gridSquareSize);
		if (gridX >= heights.size() - 1 || gridZ >= heights.size() - 1 || gridX < 0 || gridZ < 0)
			return 0;
		float xCoord = ((int)terrainX % (int)gridSquareSize) / gridSquareSize;
		float zCoord = ((int)terrainZ % (int)gridSquareSize) / gridSquareSize;

		float answer;
		if (xCoord <= (1 - zCoord)) { // posto je teren sacinjen od kvadratica, svaki od 2 trougla, ovde gledamo na kom smo tacno
			answer = Maths::barryCentric(glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ], 0),
				glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
		}
		else {
			answer = Maths::barryCentric(glm::vec3(0, heights[gridX+1][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ+1], 0),
				glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
		}

		return answer;
	}
};

#endif