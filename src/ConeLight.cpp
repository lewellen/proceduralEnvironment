#include <cstdio>
#include "OpenGL.h"

#include "ConeLight.h"

#include "Actor.h"
#include "Lights.h"
#include "Materials.h"
#include "Primitives.h"
#include "Rotation.h"
#include "Vector.h"

ConeLight::ConeLight(IFrame* frame, GLenum light, lightDefinition* definition, float spotCutoff, float spotExponent) : Actor(frame) {
	this->light = light;
	this->definition = *definition;
	this->spotCutoff = spotCutoff;
	this->spotExponent = spotExponent;

	glEnable(light);
}

ConeLight::~ConeLight() {
	glDisable(this->light);
}

void ConeLight::Render() {
	Vector* p = GetFrame()->Position();
	float lightPosition[] = { p->x, p->y, p->z, 1 };
	float lightDirection[] = { -p->x, -p->y, -p->z };
	delete p;

	// Set the light properties.
	glLightfv(light, GL_POSITION, lightPosition);

	glLightf(light, GL_SPOT_CUTOFF, spotCutoff);
	glLightf(light, GL_SPOT_EXPONENT, spotExponent);
	glLightfv(light, GL_SPOT_DIRECTION, lightDirection);

	glLightfv(light, GL_AMBIENT, this->definition.ambient);
	glLightfv(light, GL_DIFFUSE, this->definition.diffuse);
	glLightfv(light, GL_SPECULAR, this->definition.specular);

	// Visualize the light cone.
	glPushMatrix();

	// Position the cone.
	glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);

	// Orient the cone.
	Rotation* rot = GetFrame()->Orientation();
	glRotatef(rot->Yaw(), 0, 0, 1);
	glRotatef(rot->Pitch() + 90, 0, 1, 0);
	delete rot;

	// Scale it down.
	glScalef(.2, .2, .2);

	// Make it the same color as the light being projected.
	float emit[] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, this->definition.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, this->definition.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, this->definition.specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emit);

	// Render the actual cone.
	cone(0.5 * spotCutoff);

	// Reset material
	MAT_DEFAULT.Apply();

	// Draw the direction the light is pointing.
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -1);
	glEnd();

	glPopMatrix();
}