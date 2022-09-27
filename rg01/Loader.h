#ifndef H_LOADER
#define H_LOADER
#pragma once
#include "RawModel.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <iostream>

using namespace std; 

/*
	Loades textures, cube maps, VAO, VBO
*/
class Loader {
private:

	vector<unsigned int> vaos;
	vector<unsigned int> vbos;
	vector<unsigned int> textures;

	unsigned int createVAO() {
		unsigned int vaoID;
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		vaos.push_back(vaoID);

		return vaoID;
	}

	void storeDateInAttributeList(int attributeNumber, unsigned int coordinateSize, float* data, unsigned int dataSize) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);

		vbos.push_back(vboID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		//glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void storeDateInAttributeListSkyboxCube(int attributeNumber, unsigned int coordinateSize, float* data, unsigned int dataSize) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);

		vbos.push_back(vboID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		//glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void unbindVAO() {
		glBindVertexArray(0);
	}

	void bindIndicesBuffer(unsigned int* indices, int size) {
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

public:

	RawModel* loadToVAO(float* positions, unsigned int arraySize, unsigned int* indices, unsigned int indicesSize,
		float* textureCoords, unsigned int textureSize, float* normals, unsigned int normalsSize) {
		int vaoID = createVAO();
		printf("CREATING VAO %u %u %u\n", arraySize, textureSize, normalsSize);
		bindIndicesBuffer(indices, indicesSize);

		int length = indicesSize / sizeof(indices[0]);

		storeDateInAttributeList(0, 3, positions, arraySize);
		storeDateInAttributeList(1, 2, textureCoords, textureSize);
		storeDateInAttributeList(2, 3, normals, normalsSize);
		unbindVAO();
	
		return new RawModel(vaoID, length);
	}

	RawModel* loadToVAO(float* positions, unsigned int positionsSize, int dimensions) {
		int vaoID = createVAO();
		storeDateInAttributeList(0, dimensions, positions, positionsSize);
		unbindVAO();
		return new RawModel(vaoID, (positionsSize / sizeof(float)) / dimensions); // podeljeno sa 2 jer ima x i y
	}

	
	unsigned int loadTexture(char const* path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			cout << "Texture failed to load at path: " << path << endl;
			stbi_image_free(data);
		}

		textures.push_back(textureID);
		return textureID;
	}

	unsigned int loadCubeMap(std::string* faces, int facesSize) {
		unsigned int texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < facesSize; i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		// (tip, texture options, mode)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear - uzima vrednosti okolnih piksela i tu boju vraca (kod vecih rez kad se razvlace teksture)
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // setujemo magnifying i minifiying, kako kad je previse veliko i previse sitno da reaguje, reaguje linearno
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		textures.push_back(texID);
		return texID;
	}

	void cleanUp() {
		glDeleteVertexArrays(vaos.size(), &vaos[0]);
		glDeleteBuffers(vbos.size(), &vbos[0]);
		glDeleteTextures(textures.size(), &textures[0]);
	}
};
#endif