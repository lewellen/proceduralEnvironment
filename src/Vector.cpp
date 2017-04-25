#include <cstdio>
#include <cmath>

#include "Vector.h"

#define PI 3.14159265

Vector::Vector() {
	x = y = z = 0;
}

Vector::Vector(Vector* v) {
	if (v == NULL)
		return;

	x = v->x;
	y = v->y;
	z = v->z;
}

Vector::Vector(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

Vector* Vector::Cross(Vector* b) {
	Vector* v = new Vector(
		(y * b->z - z * b->y),
		-(x * b->z - z * b->x),
		(x * b->y - y * b->x)
		);

	return v;
}

float Vector::Inner(Vector* b) {
	return x * b->x + y * b->y + z * b->z;
}

float Vector::Magnitude() {
	return sqrtf(x*x + y*y + z*z);
}

Vector* Vector::Normalize() {
	float n = Magnitude();
	if (n == 0)
		return this;

	return Scale(1.0 / n);
}

void Vector::RotateAboutX(int deg) {
	float rad = deg / 180.0 * PI;
	float s = sin(rad);
	float c = cos(rad);

	float _y = y;
	float _z = z;

	y = _y * c - _z * s;
	z = _y * s + _z * c;
}

void Vector::RotateAboutY(int deg) {
	float rad = deg / 180.0 * PI;
	float s = sin(rad);
	float c = cos(rad);

	float _x = x;
	float _z = z;

	x =  _x * c + _z * s;
	z = -_x * s + _z * c;
}

void Vector::RotateAboutZ(int deg) {
	float rad = deg / 180.0 * PI;
	float s = sin(rad);
	float c = cos(rad);

	float _x = x;
	float _y = y;

	x = _x * c - _y * s;
	y = _x * s + _y * c;
}

Vector* Vector::Scale(float a) {
	x *= a;
	y *= a;
	z *= a;

	return this;
}