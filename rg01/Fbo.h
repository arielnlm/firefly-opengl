#ifndef H_FBO
#define H_FBO

#pragma once
#include <glad/glad.h>
/*
	Frame buffer object: Creates an FBO of a specified width and height, with the desired type of
	depth buffer attachment.
	Depth buffer attachment - an int indicating the type of depth buffer attachment that 
	this FBO should use.
*/
class Fbo {
public:
	unsigned int NONE = 0;
	unsigned int DEPTH_TEXTURE = 1;
	unsigned int DEPTH_RENDER_BUFFER = 2;

private:

	float SCREEN_WIDTH = 1280.0;
	float SCREEN_HEIGHT = 720.0;

	int width;
	int height;

	unsigned int frameBuffer;

	unsigned int colourTexture;
	unsigned int depthTexture;
	unsigned int depthBuffer;
	unsigned int colourBuffer;
public:

	Fbo(int width, int height, int depthBufferType) {
		this->width = width;
		this->height = height;
		initialiseFrameBuffer(depthBufferType);
	}

	void cleanUp() {
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &colourTexture);
		glDeleteTextures(1, &depthTexture);
		glDeleteRenderbuffers(1, &depthBuffer);
		glDeleteRenderbuffers(1, &colourBuffer);
	}

	void bindFrameBuffer() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, width, height);
	}

	void unbindFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	void bindToRead() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
	}

	int getColourTexture() {
		return colourTexture;
	}

	int getDepthTexture() {
		return depthTexture;
	}

private:

	void initialiseFrameBuffer(int type) {
		createFrameBuffer();
		createTextureAttachment();
		if (type == DEPTH_RENDER_BUFFER) {
			createDepthBufferAttachment();
		}
		else if (type == DEPTH_TEXTURE) {
			createDepthTextureAttachment();
		}
		unbindFrameBuffer();
	}

	void createFrameBuffer() {
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}

	void createTextureAttachment() {
		glGenTextures(1, &colourTexture);
		glBindTexture(GL_TEXTURE_2D, colourTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			(unsigned char*)0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture,
			0);
	}

	void createDepthTextureAttachment() {
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT,
			GL_FLOAT, (unsigned char*)0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	}

	void createDepthBufferAttachment() {
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
			depthBuffer);
	}

	/*unsigned int NONE = 0;
	unsigned int DEPTH_TEXTURE = 1;
	unsigned int DEPTH_RENDER_BUFFER = 2;*/

};

#endif