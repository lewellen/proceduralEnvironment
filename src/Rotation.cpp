#include <cstdio>

#include "Rotation.h"

#include "Vector.h"

Rotation::Rotation() {
	roll = pitch = yaw = 0;
}

Rotation::Rotation(Rotation* rotation) {
	if (rotation == NULL) {
		roll = pitch = yaw = 0;
		return;
	}

	roll = rotation->Roll();
	pitch = rotation->Pitch();
	yaw = rotation->Yaw();
}

Rotation::Rotation(int _roll, int _pitch, int _yaw) {
	roll = pitch = yaw = 0;
	AddRoll(_roll);
	AddPitch(_pitch);
	AddYaw(_yaw);
}

Vector* Rotation::Apply(Vector* v) {
	if (v == NULL)
		return NULL;

	v->RotateAboutX(Roll());
	v->RotateAboutY(Pitch());
	v->RotateAboutZ(Yaw());

	return v;
}

int Rotation::Roll() {
	return roll;
}

int Rotation::Pitch() {
	return pitch;
}

int Rotation::Yaw() {
	return yaw;
}

int Rotation::AddRoll(int delta) {
	return roll = addDegree(roll, delta);
}

int Rotation::AddPitch(int delta) {
	return pitch = addDegree(pitch, delta);
}

int Rotation::AddYaw(int delta) {
	return yaw = addDegree(yaw, delta);
}

int Rotation::addDegree(int x, int y) {
	x += y;
	if (x < 0)
		x += 360;
	else if (x > 360)
		x -= 360;
	return x;
}