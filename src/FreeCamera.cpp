#include <cstdio>
#include "OpenGL.h"

#include "FreeCamera.h"

#include "Rotation.h"
#include "Vector.h"

FreeCamera::FreeCamera(FreeFrame* freeFrame) {
	this->frame = freeFrame;
}

FreeCamera::~FreeCamera() {
	this->frame = NULL;
}

void FreeCamera::SetupProjection(double w2h) {
	gluPerspective(65, w2h, 1, 15);
}

void FreeCamera::SetViewMode(ViewMode viewMode) {

}

void FreeCamera::Render() {
	Vector* f = frame->Forward();
	Vector* u = frame->Up();
	Vector* p = frame->Position();

	gluLookAt(
		// Eye position
		p->x, p->y, p->z,
		// Look at position
		p->x + f->x, p->y + f->y, p->z + f->z,
		// Up vector
		u->x, u->y, u->z
		);

	delete f;
	delete u;
	delete p;

	glScalef(10, 10, 10);
}