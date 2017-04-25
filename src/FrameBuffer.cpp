#include <cstdio>
#include <stdexcept>

#include "OpenGL.h"

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height, GLenum colorAttachment, GLenum depthAttachment) {
	if (colorAttachment == GL_NONE && depthAttachment == GL_NONE)
		throw new std::invalid_argument("colorAttachment and depthAttachment cannot both be equal to GL_NONE.");

	this->width = width;
	this->height = height;

	this->colorAttachment = colorAttachment;
	this->depthAttachment = depthAttachment;

	this->colorTexture = 0;
	this->depthTexture = 0;

	// 2015-06-24 GEL Portions of this code were adapated from Example 36 and 
	// YouTube video "Series OpenGL Water Tutorial 1-5: X" by "ThinMatrix" 
	// https://www.youtube.com/watch?v=21UsMuFTN0k. And from the following 
	// OpenGL tutorial https://open.gl/framebuffers.

	if (colorAttachment != GL_NONE) {
		// Initialize an empty texture that will be filled in with the rendered scene.
		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if (depthAttachment != GL_NONE) {
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	}

	// Create the buffer that will be used in place of the regular buffer when 
	// rendering the scene.
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	if (colorAttachment != GL_NONE) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, colorTexture, 0);
		glDrawBuffer(colorAttachment);
	}

	if (depthAttachment != GL_NONE)
		glFramebufferTexture2D(GL_FRAMEBUFFER, depthAttachment, GL_TEXTURE_2D, depthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw new std::runtime_error("Failed to allocate frame buffer.");
}

FrameBuffer::~FrameBuffer() {
	if (depthAttachment != GL_NONE)
		glDeleteTextures(1, &depthTexture); 
	
	if (colorAttachment != GL_NONE)
		glDeleteTextures(1, &colorTexture);
	
	glDeleteFramebuffers(1, &frameBuffer);
}

void FrameBuffer::Begin() {
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

GLuint FrameBuffer::ColorTexture() {
	return this->colorTexture;
}

GLuint FrameBuffer::DepthTexture() {
	return this->depthTexture;
}

void FrameBuffer::End() {
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}