#ifndef H_HEIGHT_GENERATOR
#define H_HEIGHT_GENERATOR

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <random>

/*
	Generates terrain height on it's every point using adjusted Perlin Noise algorithm.
*/

class HeightGenerator {

private:
	float AMPLITUDE = 70.0f;
	int seed;
	const int OCTAVES = 2;
	const float ROUGHNESS = 0.3f;

public:

	HeightGenerator() {
		int min = 0;
		int max = 99999999;
		srand((unsigned)time(NULL));
		seed = min + (rand() % static_cast<int>(max - min + 1));	
		//seed = 13;
	}

	float generateHeight(int x, int z) {
		float total = 0;
		/*float d = glm::pow(2, OCTAVES - 1);
		for (int i = 0; i < OCTAVES; i++) {
			float freq = glm::pow(2, i) / d;
			float amp = glm::pow(ROUGHNESS, i) * AMPLITUDE;
			total += getInterpolatedNoise(x * freq, z * freq) * amp;
		}*/
		total += getInterpolatedNoise(x / 8.0, z / 8.0) * AMPLITUDE;
		//total += getInterpolatedNoise(x /4.0 , z / 4.0) * AMPLITUDE / 3.0;
		return total;
	}
	
private:
	float getNoise(int x, int z) {
		default_random_engine eng{ static_cast<long unsigned int>(x*364 + z*793 + seed) };
		std::uniform_real_distribution<> dist(0, 1);
		float value = dist(eng) * 2.0 - 1;
		return value;
	}

	float getSmoothNoise(int x, int z) {
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1) + getNoise(x + 1, z + 1)) / 16.0;
		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1) + getNoise(x, z + 1)) / 8.0;
		float center = getNoise(x, z) / 4.0;
		return corners + sides + center;
	}

	float interpolate(float a, float b, float blend) {
		double theta = blend * glm::pi<double>();
		float f = (1.0 - glm::cos(theta)) * 0.5;
		return a * (1.0 - f) + b * f;
	}

	float getInterpolatedNoise(float x, float z) {
		int intX = (int)x;
		int intZ = (int)z;
		float fracX = x - intX;
		float fracZ = z - intZ;

		float v1 = getSmoothNoise(intX, intZ);
		float v2 = getSmoothNoise(intX + 1, intZ);
		float v3 = getSmoothNoise(intX, intZ + 1);
		float v4 = getSmoothNoise(intX + 1, intZ + 1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);

		return interpolate(i1, i2, fracZ);

	}
};

#endif