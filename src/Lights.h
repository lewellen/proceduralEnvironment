#ifndef __Lights_H__
#define __Lights_H__

#include "OpenGL.h"

// Defines the attributes associated with an OpenGL light.
struct lightDefinition {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];

	void Apply(GLenum light);
};

extern struct lightDefinition LIT_NATURAL;

#endif