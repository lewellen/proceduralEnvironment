#ifndef __Materials_H__
#define __Materials_H__

#include "OpenGL.h"

// Defines the properties associated with an OpenGL material.
struct material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;

	void Apply();
};

extern struct material MAT_DEFAULT;
extern struct material MAT_LEAF;
extern struct material MAT_WATER;

#endif