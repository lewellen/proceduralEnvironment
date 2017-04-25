#ifndef __FrameBuffer_H__
#define __FrameBuffer_H__

#include "OpenGL.h"

// Wraps the OpenGL mechanics of working with a texture bound Frame buffer.
class FrameBuffer {
private:
	GLenum colorAttachment, depthAttachment;
	GLuint colorTexture, depthTexture, frameBuffer;

	int width, height;

public:
	// Initializes a new instance with the specified width and height that will 
	// associate textures with the colorAttachment and depthAttachment.
	FrameBuffer(int width, int height, GLenum colorAttachment, GLenum depthAttachment);

	// Releases memory and resources associated with the instance.
	~FrameBuffer();

	// Gets the texture handle associated with the color attachment.
	GLuint ColorTexture();

	// Gets the texture handle associated with the depth attachment.
	GLuint DepthTexture();

	// Performs the underlying OpenGL mechanics to begin using the frame 
	// buffer.
	void Begin();
	
	// Performs the underlying OpenGL mechanics to end using the frame buffer.
	void End();
};

#endif
