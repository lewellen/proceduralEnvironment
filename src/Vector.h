#ifndef __Vector_H__
#define __Vector_H__

// A three dimensional vector;
class Vector {
public:
	// The x, y, and z components of the vector.
	float x, y, z;

	// Initializes a zero vector.
	Vector();

	// Copy constructor
	Vector(Vector* vector);

	// Initializes an (x, y, z) valued vector.
	Vector(float x, float y, float z);

	// Computes the cross product of this vector with the vector b.
	Vector* Cross(Vector* b);

	// Computes the inner product of this vector with the vecot b.
	float Inner(Vector* b);

	// Gets the magnitude (length) of this vector.
	float Magnitude();

	// Normalizes this vector in place and returns the instance.
	Vector* Normalize();

	// Rotates the vector about the x-axis for the given degrees.
	void RotateAboutX(int deg);

	// Rotates the vector about the y-axis for the given degrees.
	void RotateAboutY(int deg);

	// Rotates the vector about the z-axis for the given degrees.
	void RotateAboutZ(int deg);

	// Scales this vector in place and returns the instance.
	Vector* Scale(float a);
};

#endif
