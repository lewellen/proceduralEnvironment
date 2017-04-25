#include "OpenGL.h"

#include "Materials.h"

void material::Apply() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

struct material MAT_DEFAULT = {
		{ 0.20, 0.20, 0.20, 1.00 },
		{ 1.00, 1.00, 1.00, 1.00 },
		{ 1.00, 1.00, 1.00, 1.00 },
		{ 0.00, 0.00, 0.00, 1.00 },
		1
};

struct material MAT_LEAF = {
		{ 0.00, 0.25, 0.00, 1.00 },
		{ 0.00, 0.50, 0.00, 1.00 }, 
		{ 0.00, 0.75, 0.00, 1.00 },
		{ 0.00, 0.00, 0.00, 1.00 },
		128.00
}; 

struct material MAT_WATER = {
		{ 0.00, 0.00, 0.00, 1.00 },
		{ 0.25, 0.50, 1.00, 0.50 },
		{ 1.00, 1.00, 1.00, 0.10 },
		{ 0.00, 0.00, 0.00, 1.00 },
		16.00
};