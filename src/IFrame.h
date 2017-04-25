#ifndef __IFrame_H__
#define __IFrame_H__

#include "Rotation.h"
#include "Vector.h"

// Represents a position and orientation.
class IFrame {
public:
	// Required so that G++ doesn't complain.
	virtual ~IFrame() { }
	
	// Gets the orientation.
	virtual Rotation* Orientation() = 0;

	// Gets the position.
	virtual Vector* Position() = 0;

	// Calculate the forward vector.
	virtual Vector* Forward() = 0;

	// Calculate the right vector.
	virtual Vector* Right() = 0;

	// Calculate the up vector.
	virtual Vector* Up() = 0;
};

#endif