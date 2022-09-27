#ifndef H_RAWMODEL
#define H_RAWMODEL

#pragma once
/*
	To represent model we use vaoID (vertex array object ID) and vertex count.
	Opengl uses these to draw on screen.
*/
class RawModel {

private:
	int vaoID;
	int vertexCount;

public:
	RawModel(int vaoID, int vertexCount);

	int getVaoID();
	int getVertexCount();
};

#endif