#ifndef H_MODELTEXTURE
#define H_MODELTEXUTRE

#pragma once

/*
	Stores texture of model. Transparent models and models that use fake lighting (for example grass) 
	are supported.
*/
class ModelTexture {
private:
	unsigned int textureID;

	float shineDamper = 1.0f;
	float reflectivity = 0.0f;

	bool hasTransparency = false;
	bool useFakeLighting = false;

	int numberOfRows = 1;

public:

	ModelTexture(unsigned int textureID);

	unsigned int getTextureID();
	float getShineDamper();
	float getReflectivity();
	bool isHasTransparency();
	bool isUseFakeLighting();
	int getNumberOfRows();

	void setShineDamper(float value);
	void setReflectivity(float value);
	void setHasTransparency(bool v);
	void setUseFakeLighting(bool v);
	void setNumberOfRows(int rows);
};

#endif