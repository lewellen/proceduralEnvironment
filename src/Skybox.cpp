#include <cstdio>

#include "OpenGL.h"

#include "Skybox.h"

#include "Materials.h"
#include "TextureManager.h"

Skybox::Skybox() {
	texture = TextureManager::Get()->Load(TEX_SKY);
}

Skybox::~Skybox() {

}

void Skybox::Render() {
	MAT_DEFAULT.Apply();

	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glColor3f(1, 1, 1);

	// Top
	glBegin(GL_QUADS);

	glTexCoord2f(2.0 / 3.0, 3.0 / 3.0);
	glVertex3f(0, 1, 1);

	glTexCoord2f(2.0 / 3.0, 2.0 / 3.0);
	glVertex3f(1, 1, 1);

	glTexCoord2f(1.0 / 3.0, 2.0 / 3.0);
	glVertex3f(1, 0, 1);

	glTexCoord2f(1.0 / 3.0, 3.0 / 3.0);
	glVertex3f(0, 0, 1);

	glEnd();

	// Sides
	glBegin(GL_QUAD_STRIP);

	glTexCoord2f(0.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 0, 0);
	glTexCoord2f(0.0 / 3.0, 2.0 / 3.0); glVertex3f(0, 0, 1);

	glTexCoord2f(1.0 / 3.0, 1.0 / 3.0); glVertex3f(1, 0, 0);
	glTexCoord2f(1.0 / 3.0, 2.0 / 3.0); glVertex3f(1, 0, 1);

	glTexCoord2f(2.0 / 3.0, 1.0 / 3.0); glVertex3f(1, 1, 0);
	glTexCoord2f(2.0 / 3.0, 2.0 / 3.0); glVertex3f(1, 1, 1);

	glTexCoord2f(3.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 1, 0);
	glTexCoord2f(3.0 / 3.0, 2.0 / 3.0); glVertex3f(0, 1, 1);

	glEnd();

	glBegin(GL_QUAD_STRIP);

	glTexCoord2f(1.0 / 3.0, 0.0 / 3.0); glVertex3f(0, 1, 0);
	glTexCoord2f(1.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 1, 1);

	glTexCoord2f(2.0 / 3.0, 0.0 / 3.0); glVertex3f(0, 0, 0);
	glTexCoord2f(2.0 / 3.0, 1.0 / 3.0); glVertex3f(0, 0, 1);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}