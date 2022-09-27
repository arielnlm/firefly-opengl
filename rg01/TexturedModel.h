#ifndef H_TEXTUREDMODEL
#define H_TEXTUREDMODEL

#pragma once
#include "RawModel.h"
#include "ModelTexture.h"

/*
	TexturedModel is RawModel with texture.
*/
class TexturedModel {

private:
	RawModel* rawModel;
	ModelTexture* texture;

public:
	TexturedModel(RawModel* model, ModelTexture* texture);

	RawModel* getRawModel();
	ModelTexture* getTexture();
};

#endif