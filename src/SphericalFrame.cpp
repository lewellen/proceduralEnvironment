#include <cstdio>

#include "SphericalFrame.h"

#include "IController.h"
#include "Rotation.h"
#include "Vector.h"

SphericalFrame::SphericalFrame(float r, int roll, int pitch, int yaw) {
	this->r = r;
	this->orientation = Rotation(roll, pitch, yaw);
}

SphericalFrame::~SphericalFrame() {

}

Rotation* SphericalFrame::Orientation() {
	return new Rotation(this->orientation);
}

Vector* SphericalFrame::Position() {
	return Forward()->Scale(r);
}

Vector* SphericalFrame::Forward() {
	return orientation.Apply(new Vector(1, 0, 0));
}

Vector* SphericalFrame::Right() {
	return orientation.Apply(new Vector(0, 1, 0));
}

Vector* SphericalFrame::Up() {
	return orientation.Apply(new Vector(0, 0, 1));
}