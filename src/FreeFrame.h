#ifndef __FreeFrame_H__
#define __FreeFrame_H__

#include "IFrame.h"
#include "Rotation.h"
#include "Vector.h"

// Represents a free-form frrame where both position and orientation can be modified 
// unconstrained.
class FreeFrame : public IFrame {
public:
	Vector position;
	Rotation orientation;

	// Initializes a new instance with the given properties.
	FreeFrame(float x, float y, float z, int roll, int pitch, int yaw);

	// Frees any memory associated with the instance.
	~FreeFrame();

	// Inherited from IFrame. Retrieves the orientation.
	Rotation* Orientation();

	// Inherited from IFrame. Retrieves the position.
	Vector* Position();

	// Inherited from IFrame. Calculates the forward vector.
	Vector* Forward();

	// Inherited from IFrame. Calculates the right vector.
	Vector* Right();

	// Inherited from IFrame. Calculates the up vector.
	Vector* Up();
};

#endif