#include <cstdio>

#include "ToggleController.h"

#include "IController.h"

ToggleController::ToggleController(IController* whenNotToggled, IController* whenToggled) {
	this->toggled = false;
	this->whenToggled = whenToggled;
	this->whenNotToggled = whenNotToggled;
}

void ToggleController::Toggle() {
	toggled = !toggled;
}

void ToggleController::SetToggled() {
	toggled = true;
}

void ToggleController::SetUntoggled() {
	toggled = false;
}

void ToggleController::HandleKey(unsigned char key) {
	if (toggled)
		whenToggled->HandleKey(key);
	else
		whenNotToggled->HandleKey(key);
}

void ToggleController::HandleSpecialKey(int key) {
	if (toggled)
		whenToggled->HandleSpecialKey(key);
	else
		whenNotToggled->HandleSpecialKey(key);
}