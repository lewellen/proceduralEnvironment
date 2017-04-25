#include <cstdio>

#include "FreeFrame.h"

#include "FreeController.h"
#include "IController.h"
#include "Rotation.h"
#include "Vector.h"

FreeFrame::FreeFrame(float x, float y, float z, int roll, int pitch, int yaw) {
	this->position = Vector(x, y, z);
	this->orientation = Rotation(roll, pitch, yaw);
}

FreeFrame::~FreeFrame() {

}

Rotation* FreeFrame::Orientation() {
	return new Rotation(this->orientation);
}

Vector* FreeFrame::Position() {
	return new Vector(this->position);
}

Vector* FreeFrame::Forward() {
	return orientation.Apply(new Vector(1, 0, 0));
}

Vector* FreeFrame::Right() {
	return orientation.Apply(new Vector(0, 1, 0));
}

Vector* FreeFrame::Up() {
	return orientation.Apply(new Vector(0, 0, 1));
}