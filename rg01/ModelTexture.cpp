#include "ModelTexture.h"

ModelTexture::ModelTexture(unsigned int textureID) {
	this->textureID = textureID;
}

unsigned int ModelTexture::getTextureID() {
	return this->textureID;
}

float ModelTexture::getShineDamper() {
	return shineDamper;
}

float ModelTexture::getReflectivity() {
	return reflectivity;
}

void ModelTexture::setShineDamper(float value) {
	shineDamper = value;
}

void ModelTexture::setReflectivity(float value) {
	reflectivity = value;
}

bool ModelTexture::isHasTransparency() {
	return hasTransparency;
}

void ModelTexture::setHasTransparency(bool v) {
	hasTransparency = v;
}

bool ModelTexture::isUseFakeLighting() {
	return useFakeLighting;
}

int ModelTexture::getNumberOfRows()
{
	return numberOfRows;
}

void ModelTexture::setUseFakeLighting(bool v) {
	useFakeLighting = v;
}

void ModelTexture::setNumberOfRows(int rows)
{
	numberOfRows = rows;
}

