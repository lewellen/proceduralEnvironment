#ifndef __SphericalController_H__
#define __SphericalController_H__

#include "IController.h"
#include "SphericalFrame.h"

// Keyboard controls for manipulating a spherical frame.
class SphericalController : public IController {
private:
	SphericalFrame* frame;

public:
	// Initializes a new instance based on the frame.
	SphericalController(SphericalFrame* frame);

	// Releases any memory associated with the instance.
	~SphericalController();

	// Inherited from IController. Handles regular keyboard input.
	void HandleKey(unsigned char key);

	// Inherited from IController. Handles special keyboard input.
	void HandleSpecialKey(int key);
};

#endif