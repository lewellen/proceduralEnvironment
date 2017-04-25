#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include "Vector.h"
#include "Primitives.h"

#define PI 3.14159265359

void cone(float angle) {
	cone(tan(angle / 180.0 * PI), 1.0);
}

void cone(float radius, float height) {
	float dTheta = 30;

	float x = 0;
	float y = 0;

	// Close the cone.
	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(0, 0, -1);

	glVertex3f(0, 0, 0);
	for (float theta = 0; theta <= 360.0; theta += dTheta) {
		float rad = -theta / 180.0 * PI;
		glVertex3f(radius * cos(rad), radius * sin(rad), 0.0);
	}

	glEnd();

	// The cone itself
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, height);
	for (float theta = 0; theta <= 360.0; theta += dTheta) {
		float rad = theta / 180.0 * PI;
		float x = radius * cos(rad);
		float y = radius * sin(rad);

		glNormal3f(x, y, height);
		glVertex3f(x, y, 0);
	}

	glEnd();
}

void cube(float x, float y, float z, bool centered) {
	float x0 = 0.0, y0 = 0.0, z0 = 0.0;
	if (centered) {
		x0 = -0.5 * x;
		y0 = -0.5 * y;
		z0 = -0.5 * z;

		glPushMatrix();
		glTranslatef(x0, y0, z0);
	}

	glBegin(GL_QUADS);

	// Top face
	glNormal3f(0, 0, +1);
	glVertex3f(0, 0, z);
	glVertex3f(x, 0, z);
	glVertex3f(x, y, z);
	glVertex3f(0, y, z);

	// Bottom face
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, 0, 0);

	// Left face
	glNormal3f(0, -1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, z);
	glVertex3f(0, 0, z);

	// Right face
	glNormal3f(0, +1, 0);
	glVertex3f(0, y, z);
	glVertex3f(x, y, z);
	glVertex3f(x, y, 0);
	glVertex3f(0, y, 0);

	// Front face
	glNormal3f(+1, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	glVertex3f(x, 0, z);

	// Back face
	glNormal3f(-1, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);

	glEnd();

	if (centered)
		glPopMatrix();
}

void cylinder(float radius, float height) {

}