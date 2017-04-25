#include <cstdio>

#include "OpenGL.h"


#include "SphericalController.h"

#include "SphericalFrame.h"

SphericalController::SphericalController(SphericalFrame* frame) {
	this->frame = frame;
}

SphericalController::~SphericalController() {
	this->frame = NULL;
}

void SphericalController::HandleKey(unsigned char key) {
	// Zoom in
	if (key == 'w')
		frame->r -= .05;

	// Zoom out
	if (key == 's')
		frame->r += .05;
}

void SphericalController::HandleSpecialKey(int key) {
	int rotateDegrees = 2;

	switch (key) {
	case GLUT_KEY_RIGHT:
		frame->orientation.AddYaw(rotateDegrees);
		break;
	case GLUT_KEY_LEFT:
		frame->orientation.AddYaw(-rotateDegrees);
		break;
	case GLUT_KEY_DOWN:
		frame->orientation.AddPitch(rotateDegrees);
		break;
	case GLUT_KEY_UP:
		frame->orientation.AddPitch(-rotateDegrees);
		break;
	}
}