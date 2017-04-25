#ifndef __SphericalFrame_H__
#define __SphericalFrame_H__

#include "IFrame.h"
#include "Rotation.h"
#include "Vector.h"

// Represents a spherical position and orientation where movement takes place 
// on a sphere with a given radius looking in at the origin.
class SphericalFrame : public IFrame {
public:
	float r;
	Rotation orientation;

	// Initializes a new instance given the properties.
	SphericalFrame(float r, int roll, int pitch, int yaw);
	
	// Releases any memory associated with the instance.
	~SphericalFrame();

	// Gets the orientation.
	Rotation* Orientation();

	// Gets the position.
	Vector* Position();

	// Inherited from IFrame. Calculates for the forward vector.
	Vector* Forward();

	// Inherited from IFrame. Calculates the right vector.
	Vector* Right();

	// Inherited from IFrame. Calculates the up vector.
	Vector* Up();
};

#endif