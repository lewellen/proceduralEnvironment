#ifndef __ToggleController_H__
#define __ToggleController_H__

#include "IController.h"

// Allows the toggling between two controllers.
class ToggleController : public IController {
private:
	bool toggled;
	IController* whenToggled;
	IController* whenNotToggled;

public:
	// Initializes the instance with the given controllers.
	ToggleController(IController* whenNotToggled, IController* whenToggled);

	// Toggles between the underlying controllers.
	void Toggle();

	// Uses the toggled controller.
	void SetToggled();

	// Uses the untoggled controller.
	void SetUntoggled();

	// Inherited from IController. Handles regular keyboard input.
	void HandleKey(unsigned char key);

	// Inherited from IController. Handles special keyboard input.
	void HandleSpecialKey(int key);
};

#endif