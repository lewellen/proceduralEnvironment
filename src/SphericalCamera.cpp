#include <cstdio>

#include "OpenGL.h"


#include <stdexcept>

#include "SphericalCamera.h"

#include "SphericalFrame.h"
#include "Vector.h"

SphericalCamera::SphericalCamera(SphericalFrame* frame, ViewMode viewMode) {
	this->frame = frame;
	this->viewMode = viewMode;
}

SphericalCamera::~SphericalCamera() {
	this->frame = NULL;
}

void SphericalCamera::SetupProjection(double w2h) {
	if (viewMode == VM_PERSPECTIVE) {
		gluPerspective(65, w2h, .05, 3.0);
	}
	else if (viewMode == VM_ORTHOGONAL) {
		float dim = 2.5;

		glOrtho(-w2h * dim, +w2h * dim, -dim, dim, -2 * dim, 2*dim);
	}
	else {
		throw new std::invalid_argument("viewMode must be VM_PERSPECTIVE or VM_ORTHOGONAL.");
	}
}

void SphericalCamera::SetViewMode(ViewMode viewMode) {
	// If there's no actual change, then do nothing.
	if (this->viewMode == viewMode)
		return;

	// Adjust the zoom so that scale is about the same
	if (this->viewMode == VM_ORTHOGONAL) {
		// VM_ORTHOGONAL -> VM_PERSPECTIVE
		frame->r *= 3;
	}
	else {
		// VM_PERSPECTIVE -> VM_ORTHOGONAL
		frame->r /= 3.0;
	}

	// Finally, set the internal value. Assume that the change in display is 
	// done at main.cpp level.
	this->viewMode = viewMode;
}

void SphericalCamera::Render() {
	// Calculate where the camera should be.
	Vector* forward = frame->Forward();
	Vector* up = frame->Up();
	
	gluLookAt(
			// Eye position
			forward->x, forward->y, forward->z,
			// Look at position
			0, 0, 0,
			// Up vector
			up->x, up->y, up->z
			);

	 //Zoom in or out.
	float r = frame->r;
	glScalef(1 / r, 1 / r, 1 / r);

	delete forward;
	delete up;
}