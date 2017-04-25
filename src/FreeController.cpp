#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include "FreeController.h"

#include "IController.h"
#include "Rotation.h"
#include "Vector.h"

FreeController::FreeController(FreeFrame* frame) {
	this->frame = frame;
}

FreeController::~FreeController() {
	this->frame = NULL;
}

void FreeController::HandleKey(unsigned char key) {
	// Navigation controls.
	if (key == 'w' || key == 's') {
		Vector* lookAt = this->frame->Forward();

		// forward
		int sign = 1;
		if (key == 's')
			// backward
			sign = -1;

		frame->position.x += sign * 0.2 * lookAt->x;
		frame->position.y += sign * 0.2 * lookAt->y;
		frame->position.z += sign * 0.2 * lookAt->z;

		delete lookAt;
	}

	if (key == 'a' || key == 'd') {
		Vector* strafe = this->frame->Right();
		// left
		int sign = -1;
		if (key == 'a')
			// right
			sign = 1;

		frame->position.x += sign * 0.2 * strafe->x;
		frame->position.y += sign * 0.2 * strafe->y;
		frame->position.z += sign * 0.2 * strafe->z;

		delete strafe;
	}

	if (key == 'r' || key == 'f') {
		// up
		int sign = 1;
		if (key == 'f')
			// down
			sign = -1;

		frame->position.z += sign * 0.2;
	}

	float bounds = 4;
	frame->position.x = fmaxf(-bounds, fminf(bounds, frame->position.x));
	frame->position.y = fmaxf(-bounds, fminf(bounds, frame->position.y));
	frame->position.z = fmaxf(-bounds, fminf(bounds, frame->position.z));
}

void FreeController::HandleSpecialKey(int key) {
	int rotateDegrees = 2;

	switch (key) {
	case GLUT_KEY_UP:
		// Rotate about the Y axis CCW
		frame->orientation.AddPitch(-rotateDegrees);
		break;
	case GLUT_KEY_DOWN:
		// Rotate about the Y axis CW
		frame->orientation.AddPitch(rotateDegrees);
		break;
	case GLUT_KEY_RIGHT:
		// Rotate about the Z axis CW
		frame->orientation.AddYaw(-rotateDegrees);
		break;
	case GLUT_KEY_LEFT:
		// Rotate about the Z axis CCW
		frame->orientation.AddYaw(rotateDegrees);
		break;
	}
}