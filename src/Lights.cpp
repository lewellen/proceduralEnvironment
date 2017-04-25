#include "OpenGL.h"

#include "Lights.h"

void lightDefinition::Apply(GLenum light) {
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);
	glLightfv(light, GL_EMISSION, emission);
}

struct lightDefinition LIT_NATURAL = {
		{ 0.50, 0.50, 0.50, 1.00 },
		{ 0.75, 0.75, 0.75, 1.00 },
		{ 0.90, 0.90, 0.90, 1.00 },
		{ 0.00, 0.00, 0.00, 1.00 }
};