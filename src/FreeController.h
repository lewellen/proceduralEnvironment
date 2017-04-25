#ifndef __FreeController_H__
#define __FreeController_H__

#include "FreeFrame.h"
#include "IController.h"

// Keyboard controls for operating in "free" mode.
class FreeController : public IController {
private:
	FreeFrame* frame;

public:
	// Initializes a new instance based on the position and orientation given 
	// by the frame.
	FreeController(FreeFrame* frame);
	
	// Releases any allocated memory.
	~FreeController();

	// Inherited from IController. Modifies the frame based on keyboard input.
	void HandleKey(unsigned char key);

	// Inherited from IController. Modifies the frame based on keyboard input.
	void HandleSpecialKey(int key);
};

#endif