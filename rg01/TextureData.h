#ifndef H_TEXTURE_DATA
#define H_TEXTURE_DATA

#pragma once

/*
	Stores basic texture info: width and height
*/
class TextureData {
private:
	int width;
	int height;

public:
	TextureData(int width, int height) {
		this->width = width;
		this->height = height;
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
};

#endif