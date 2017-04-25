#ifndef __IController_H__
#define __IController_H__

// Keyboard based controls.
class IController {
public:
	// Required so that G++ doesn't complain.
	virtual ~IController() { }

	// Performs some action based on a regular keyboard input (a,b,c keys etc.)
	virtual void HandleKey(unsigned char key) = 0;

	// Performs some action based on a special keyboard input (arrow keys etc.)
	virtual void HandleSpecialKey(int key) = 0;
};

#endif