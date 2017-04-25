#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include "Butterfly.h"
#include "Primitives.h"
#include "Random.h"
#include "TextureManager.h"

Butterfly::Butterfly() {
	TextureKey key = TEX_BUTTERFLY;
	switch (Random::Get()->NextInt(3)) {
	case 0:
		key = TEX_BUTTERFLY;
		break;
	case 1:
		key = TEX_BUTTERFLY2;
		break;
	default:
		key = TEX_BUTTERFLY3;
		break;
	}

	textureHandle = TextureManager::Get()->Load(key);

	position = Vector();
	position.x = Random::Get()->NextFloat();
	position.y = Random::Get()->NextFloat();
	position.z = Random::Get()->NextFloat();

	target = Vector();
	target.x = Random::Get()->NextFloat();
	target.y = Random::Get()->NextFloat();
	target.z = Random::Get()->NextFloat();

	targetDirection = Vector();
	targetDirection.x = 0.5 - Random::Get()->NextFloat();
	targetDirection.y = 0.5 - Random::Get()->NextFloat();
	targetDirection.z = 0.5 - Random::Get()->NextFloat();

	butterflySpeed = 0.25 * 0.011;
	targetSpeed =    0.50 * 0.009;
	elapsedTimeSteps = 0;
}

Butterfly::~Butterfly() {

}

void Butterfly::DoIdle() {
	float distance = sqrtf(
		powf(position.x - target.x, 2.0) +
		powf(position.y - target.y, 2.0) +
		powf(position.z - target.z, 2.0)
		);

	if (
		distance < 0.01
		|| target.x < 0 || target.x >= 1
		|| target.y < 0 || target.y >= 1
		|| target.z < 0 || target.z >= 1
		) {

		// If the butterfly catches the target, or the target escapes, create a 
		// new target.
		target.x = Random::Get()->NextFloat();
		target.y = Random::Get()->NextFloat();
		target.z = Random::Get()->NextFloat();

		targetDirection.x = 0.5 - Random::Get()->NextFloat();
		targetDirection.y = 0.5 - Random::Get()->NextFloat();
		targetDirection.z = 0.5 - Random::Get()->NextFloat();
	}

	// Update the target's position
	target.x += targetSpeed * targetDirection.x;
	target.y += targetSpeed * targetDirection.y;
	target.z += targetSpeed * targetDirection.z;

	Vector direction = Vector(
		target.x - position.x,
		target.y - position.y,
		target.z - position.z
		);

	direction.Normalize();

	// Assume that the butterfly has some smarts to anticipate where the target 
	// will go so that it can go in that direction accordingly.
	position.x += direction.x * butterflySpeed;
	position.y += direction.y * butterflySpeed;
	position.z += direction.z * butterflySpeed;

	elapsedTimeSteps++;
}

void Butterfly::Render() {
	// Used to determine the wing rotation
	int degree = (int)((0.5 - (((8 * elapsedTimeSteps) % 100) / 100.0)) * 90);

	// Direction the butterfly is traveling
	Vector direction = Vector(
		target.x - position.x,
		target.y - position.y,
		target.z - position.z
		);

	direction.Normalize();

	// Go from direction vector to euler angles so that glRotates can be 
	// applied to model.
	int pitch = atanf(direction.z) / 3.14159 * 180.0;

	int yaw = 0;
	if (direction.x < 0) {
		if (direction.y < 0) {
			yaw = 180.0 + atanf(direction.y / direction.x) / 3.14159 * 180.0;
		}
		else if (direction.y == 0) {
			yaw = 180;
		}
		else {
			yaw = 180 - atanf(-direction.y / direction.x) / 3.14159 * 180.0;
		}
	}
	else if (direction.x == 0) {
		if (direction.y < 0) {
			yaw = 270;
		}
		else if (direction.y == 0) {
			yaw = 0; // :/
		}
		else {
			yaw = +90;
		}
	}
	else {
		if (direction.y < 0) {
			yaw = 360 - atan(-direction.y / direction.x) / 3.14159 * 180.0;
		}
		else if (direction.y == 0) {
			yaw = 0;
		}
		else {
			yaw = atanf(direction.y / direction.x) / 3.14159 * 180.0;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glScalef(1.0 / 65.0, 1.0 / 65.0, 1.0 / 65.0);
		glRotatef(pitch, 0, 1, 0);
		glRotatef(yaw, 0, 0, 1);

		glPushMatrix();
			glRotatef(degree, 1, 0, 0);

			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glTexCoord2f(.5, 0); glVertex3f(-.2, 0, 0);
				glTexCoord2f(.5, 1); glVertex3f(.2, 0, 0);
				glTexCoord2f(1, 1); glVertex3f(.2, .2, 0);
				glTexCoord2f(1, 0); glVertex3f(-.2, .2, 0);
			glEnd();
		glPopMatrix();

		glPushMatrix();
			glRotatef(-degree, 1, 0, 0);

			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glTexCoord2f(0, 0); glVertex3f(-.2, -.2, 0);
				glTexCoord2f(0, 1); glVertex3f(.2, -.2, 0);
				glTexCoord2f(.5, 1); glVertex3f(.2, 0, 0);
				glTexCoord2f(.5, 0); glVertex3f(-.2, 0, 0);
			glEnd();
		glPopMatrix();
	glPopMatrix();

	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}