#ifndef __Rotation_H__
#define __Rotation_H__

#include "Vector.h"

// Represents the roll, pitch and yaw of an object in space. All rotations are 
// in degrees between [0, 360).
class Rotation {
private:
	int roll, pitch, yaw;
	int addDegree(int x, int y);

public:
	// Initializes a new instance with 0 rotation.
	Rotation();

	// Copy constructor
	Rotation(Rotation* rotation);

	// Initializes a new instance with the given roll, pitch, and yaw.
	Rotation(int _roll, int _pitch, int _yaw);

	// Applies the rotation to the vector.
	Vector* Apply(Vector* vector);

	// Gets the roll (rotation about x-axis).
	int Roll();

	// Gets the pitch (rotation about y-axis).
	int Pitch();

	// Gets the yaw (rotation about z-axis).
	int Yaw();

	// Increments the roll by the given delta.
	int AddRoll(int delta);

	// Increments the pitch by the given delta.
	int AddPitch(int delta);

	// Increments the yaw by the given delta.
	int AddYaw(int delta);
};


#endif
