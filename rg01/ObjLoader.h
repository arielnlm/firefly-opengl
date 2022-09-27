#ifndef H_OBJ_LOADER
#define H_OBJ_LOADER

#pragma once
#include "Loader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*
	Parses .obj files into vertecies, texture coordinates, normals
*/
class ObjLoader {

private:
	static void processVertex(vector<string> vertexData, vector<unsigned int>& indices, vector<glm::vec2> textures, vector<glm::vec3> normals, float* textureArray, float* normalsArray) {
		int currentVertexPointer = stoi(vertexData[0]) - 1;
		//printf("%d ", currentVertexPointer);
		indices.push_back(currentVertexPointer);
		glm::vec2 currentTex = textures[stoul(vertexData[1])-1];
		textureArray[currentVertexPointer * 2] = currentTex.x;
		textureArray[currentVertexPointer * 2 + 1] = 1 - currentTex.y;
		glm::vec3 currentNorm = normals[stoul(vertexData[2]) - 1];
		normalsArray[currentVertexPointer * 3] = currentNorm.x;
		normalsArray[currentVertexPointer * 3+1] = currentNorm.y;
		normalsArray[currentVertexPointer * 3+2] = currentNorm.z;
	}

	static vector<string> split(string str, string token) {
		vector<string>result;
		while (str.size()) {
			int index = str.find(token);
			if (index != string::npos) {
				result.push_back(str.substr(0, index));
				str = str.substr(index + token.size());
				if (str.size() == 0)result.push_back(str);
			}
			else {
				result.push_back(str);
				str = "";
			}
		}
		return result;
	}

public:
	static RawModel* loadObjModel(const char* fileName, Loader* loader) {
		printf("LOADING OBJECT\n");
		ifstream fr;
		fr.open(fileName);
		if (!fr) {
			cerr << "Error file could not be opened" << endl;
			return NULL;
		}
		string line;
		vector<glm::vec3> vertices;
		vector<glm::vec2> textures;
		vector<glm::vec3> normals;
		vector<unsigned int> indices;
		float* verticesArray;
		float* normalsArray = nullptr;
		float* textureArray = nullptr;
		unsigned int* indicesArray;

		while (true) {
			getline(fr, line);
			vector<string> splits = split(line, " ");
			if (line.rfind("v ", 0) == 0) {
				
				glm::vec3 vertex = glm::vec3(stof(splits[1]), stof(splits[2]), stof(splits[3]));
				vertices.push_back(vertex);
			}
			else if (line.rfind("vt ", 0) == 0) {
				glm::vec2 texture = glm::vec2(stof(splits[1]), stof(splits[2]));
				textures.push_back(texture);
			}
			else if (line.rfind("vn ", 0) == 0) {
				glm::vec3 normal = glm::vec3(stof(splits[1]), stof(splits[2]), stof(splits[3]));
				normals.push_back(normal);
			}
			else if (line.rfind("f ", 0) == 0) {
				textureArray = new float[vertices.size() * 2];
				normalsArray = new float[vertices.size() * 3];
				break;
			}
		}
		int textureSizeCounter = 0;
		while (!fr.eof()) {

			if (line.rfind("f ", 0) != 0) {
				getline(fr, line);
				continue;
			}
			vector<string> splits;
			splits = split(line, " ");

			vector<string> vertex1, vertex2, vertex3;
			vertex1 = split(splits[1], "/");
			vertex2 = split(splits[2], "/");
			vertex3 = split(splits[3], "/");
			
			processVertex(vertex1, indices, textures, normals, textureArray, normalsArray);
			processVertex(vertex2, indices, textures, normals, textureArray, normalsArray);
			processVertex(vertex3, indices, textures, normals, textureArray, normalsArray);
			textureSizeCounter += 6;
			getline(fr, line);

		}

		fr.close();

		verticesArray = new float[vertices.size() * 3];
		indicesArray = new unsigned int[indices.size()];

		unsigned int vertexPointer = 0;
		for (unsigned int i = 0; i < vertices.size(); i++) {
			verticesArray[vertexPointer++] = vertices[i].x;
			verticesArray[vertexPointer++] = vertices[i].y;
			verticesArray[vertexPointer++] = vertices[i].z;
		}
		unsigned int indicesIndex = 0;
		for (indicesIndex = 0; indicesIndex < indices.size(); indicesIndex++) {
			indicesArray[indicesIndex] = indices[indicesIndex];
		}
		
		printf("Lenghts: %u %u %u %d %d\n", vertexPointer, indicesIndex, textureSizeCounter, textures.size(), normals.size());
		/*ROOT OF ALL EVIL*/
		// printf("%d BEFORE CREATING VAO %u %u %u\n",tcnt, sizeof(indicesArray) , sizeof(textureArray), sizeof(normalsArray));
		return loader->loadToVAO(verticesArray, vertexPointer * sizeof(float), indicesArray, indicesIndex * sizeof(unsigned int),
			textureArray, vertices.size() * 2 /*textures.size()*/ * sizeof(float), 
			normalsArray, vertices.size() * 3 /*normals.size()*/ * sizeof(float));
	}
};

#endif