#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include "House.h"

#include "Materials.h"
#include "Placeable.h"
#include "Primitives.h"
#include "SmokePlume.h"
#include "TextureManager.h"

#define PI 3.14159265

House::House(int rotation, float scale) : Placeable() {
	this->rotation = rotation;
	this->scale = scale;

	this->logTexture = TextureManager::Get()->Load(TEX_LOG);
	this->logEndTexture = TextureManager::Get()->Load(TEX_LOG_END);
	this->roofTexture = TextureManager::Get()->Load(TEX_ROOF);
	this->roofSideTexture = TextureManager::Get()->Load(TEX_ROOF_EDGE);
	this->doorTexture = TextureManager::Get()->Load(TEX_DOOR);
	this->chimneyTexture = TextureManager::Get()->Load(TEX_CHIMNEY);
	this->windowTexture = TextureManager::Get()->Load(TEX_WINDOW);

	smokeEnabled = false;

	plume = new SmokePlume();
}

void House::ToggleSmoke() {
	smokeEnabled = !smokeEnabled;
}

void House::DoIdle() {
	if (smokeEnabled)
		plume->DoIdle();
}

int House::GetRotation() {
	return this->rotation;
}

float House::GetScale() {
	return this->scale;
}

int House::GetRowFootPrint() {
	return (int)(this->scale * 14);
}

int House::GetColumnFootPrint() {
	return (int)(this->scale * 14);
}

void House::Render() {
	MAT_DEFAULT.Apply();

	glPushMatrix();

	glTranslatef(.1, .1, 0.0625);

	// left wall (along x axis)
	glPushMatrix();
	glTranslatef(-.1, 0, 0);
	wall(6);
	glPopMatrix();

	// window
	glPushMatrix();

	glTranslatef(.5*(.8 - 5 * 0.0625), 0, .5*(.8 - 5 * 0.0625));

	glRotatef(90, 1, 0, 0);

	texturedCube(5 * 0.0625, 5 * 0.0625, .0625, windowTexture, roofSideTexture);

	glPopMatrix();

	// back wall (along y axis);
	glPushMatrix();
	glTranslatef(0, .9, 0);
	glRotatef(-90, 0, 0, 1);
	wall(6);

	glTranslatef(0, 0, 10 * 0.0625);
	wedge();

	glPopMatrix();


	// Chimney
	glPushMatrix();

	glTranslatef(-.25, .5*(.8 - .25), -0.0625);

	for (int i = 0; i < 5; i++) {
		texturedCube(.25, .25, .25, chimneyTexture, chimneyTexture);
		glTranslatef(0, 0, .25);
	}

	glTranslatef(-.025, -.025, 0);
	texturedCube(.30, .30, .05, chimneyTexture, chimneyTexture);

	glTranslatef(+.025 + .125, +.025 + .125, .05);
	chimneyTop();

	if (smokeEnabled) {
		glScalef(.4, .4, .4);
		glTranslatef(0, 0, .05);
		plume->Render();
	}

	glPopMatrix();


	// right wall (parallel to x axis)
	glPushMatrix();
	glTranslatef(-.1, .8, 0);
	wall(6);
	glPopMatrix();



	//Front wall (parallel to y axis)
	glPushMatrix();
	glTranslatef(.8, -.1, 0);
	glRotatef(90, 0, 0, 1);
	wall(6);

	glTranslatef(0, 0, 10 * 0.0625);
	wedge();

	glPopMatrix();



	// front door
	glPushMatrix();

	glTranslatef(.8, .5*(.8 - 5 * 0.0625), -0.0625);

	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	texturedCube(5 * 0.0625, 10 * 0.0625, .0625, doorTexture, roofSideTexture);

	glPopMatrix();



	// Left side of roof
	float sideLength = .9 * sqrt(2) / 2.0;
	glPushMatrix();
	glTranslatef(-.2, 0, 10 * 0.0625);
	glRotatef(45, 1, 0, 0);
	texturedCube(1.2, sideLength, 0.125, roofTexture, roofSideTexture);
	glPopMatrix();



	// right side of roof
	glPushMatrix();
	glTranslatef(1, .8, 10 * 0.0625);
	glRotatef(180, 0, 0, 1);
	glRotatef(45, 1, 0, 0);
	texturedCube(1.2, sideLength, 0.125, roofTexture, roofSideTexture);
	glPopMatrix();

	glPopMatrix();
}

void House::texturedCube(float x, float y, float z, int topTexture, int sideTextures) {
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Top face
	glPushMatrix();
	glTranslatef(0, 0, z);
	texturedQuad(x, y, topTexture);
	glPopMatrix();

	// Bottom face (no texturing)
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, 0, 0);
	glEnd();

	// Left face
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	texturedQuad(x, z, sideTextures);
	glPopMatrix();

	// Right face
	glPushMatrix();
	glTranslatef(0, y, z);
	glRotatef(-90, 1, 0, 0);
	texturedQuad(x, z, sideTextures);
	glPopMatrix();

	// Front face
	glPushMatrix();
	glTranslatef(x, 0, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	texturedQuad(y, z, sideTextures);
	glPopMatrix();

	// Back face
	glPushMatrix();
	glTranslatef(0, y, 0);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	texturedQuad(y, z, sideTextures);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void House::texturedQuad(float width, float height, int texture) {
	float segments = 7;
	float dSegment = 1.0 / segments;

	float dW = width / segments;
	float dH = height / segments;

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	for (int i = 0; i < segments; i++) {
		for (int j = 0; j < segments; j++) {
			glTexCoord2f(dSegment * i, dSegment * j);
			glVertex3f(dW * i, dH * j, 0);

			glTexCoord2f(dSegment * (i + 1), dSegment * j);
			glVertex3f(dW * (i + 1), dH * j, 0);

			glTexCoord2f(dSegment * (i + 1), dSegment * (j + 1));
			glVertex3f(dW * (i + 1), dH * (j + 1), 0);

			glTexCoord2f(dSegment * i, dSegment * (j + 1));
			glVertex3f(dW * i, dH * (j + 1), 0);
		}
	}

	glEnd();
}

void House::wedge() {
	int logs = 4;
	float radius = 0.0625;
	float length = 1.0;

	for (int i = 0; i < logs; i++) {
		glPushMatrix();
		glTranslatef(2 * i*radius, 0, 2 * i*radius);
		log(length - i * 4 * radius, radius);
		glPopMatrix();
	}
}

void House::wall(int logs) {
	float radius = 0.0625;
	float length = 1.0;

	for (int i = 0; i < logs; i++) {
		glPushMatrix();
		glTranslatef(0, 0, 2 * i*radius);
		log(length, radius);
		glPopMatrix();
	}
}

void House::chimneyTop() {
	glDisable(GL_CULL_FACE);

	int segments = 1;
	int faces = 12;

	float length = .0625;
	float radius = .06;

	float dTheta = 360.0 / faces;

	float dSegment = length / (float)segments;

	glBegin(GL_QUAD_STRIP);

	for (int theta = 0; theta <= 360; theta += dTheta) {
		float c = cos(theta / 180.0 * PI);
		float s = sin(theta / 180.0 * PI);

		glNormal3f(c, s, 0);
		glVertex3f(radius * c, radius * s, dSegment);
		glVertex3f(radius * c, radius * s, 0);
	}

	glEnd();

	glNormal3f(0, 0, 1);
	glVertex3f(0, 0, 0);

	glBegin(GL_TRIANGLE_FAN);
	for (int theta = 0; theta <= 360; theta += dTheta) {
		float c = cos(theta / 180.0 * PI);
		float s = sin(theta / 180.0 * PI);

		glVertex3f(radius * c, radius * s, 0.01);
	}
	glEnd();

	glEnable(GL_CULL_FACE);
}

void House::log(float length, float radius) {
	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, logTexture);

	int segments = 20;
	int faces = 8;

	int dTheta = 360 / faces;

	float dSegment = length / (float)segments;

	// Shaft
	for (int segment = 0; segment < segments; segment++) {
		glBegin(GL_QUAD_STRIP);

		// Shaft segment generated by revolving line segments about the x-axis.
		for (int theta = 0; theta <= 360; theta += dTheta) {
			float lengthOffset = segment * dSegment;
			float c = cos(theta / 180.0 * PI);
			float s = sin(theta / 180.0 * PI);

			glNormal3f(0, c, s);

			glTexCoord2f((segment + 1) / (float)segments, theta / 360.0);
			glVertex3f(lengthOffset + dSegment, radius * c, radius * s);

			glTexCoord2f(segment / (float)segments, theta / 360.0);
			glVertex3f(lengthOffset, radius * c, radius * s);
		}

		glEnd();
	}

	glBindTexture(GL_TEXTURE_2D, logEndTexture);

	// back cap
	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(-1, 0, 0);

	glTexCoord2f(.5, .5);
	glVertex3f(0, 0, 0);

	for (int theta = 0; theta <= 360; theta += dTheta) {
		float c = cos(theta / 180.0 * PI);
		float s = sin(theta / 180.0 * PI);

		glTexCoord2f(0.5*(1 + .5*s), 0.5*(1 + .5*c));
		glVertex3f(0, radius * s, radius * c);
	}

	glEnd();

	// Front cap
	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(1, 0, 0);

	glTexCoord2f(.5, .5);
	glVertex3f(length, 0, 0);

	for (int theta = 0; theta <= 360; theta += dTheta) {
		float c = cos(-theta / 180.0 * PI);
		float s = sin(-theta / 180.0 * PI);

		glTexCoord2f(0.5*(1 + .5*s), 0.5*(1 + .5*c));
		glVertex3f(length, radius * s, radius * c);
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}