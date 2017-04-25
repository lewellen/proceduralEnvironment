#ifndef __FreeCamera_H__
#define __FreeCamera_H__

#include "FreeFrame.h"
#include "ICamera.h"

// A camera that is free to wander and investigate the world without limitation.
class FreeCamera : public ICamera {
private :
	FreeFrame* frame;

public:
	// Initialize a new instance of the FreeCamera with default values.
	FreeCamera(FreeFrame* freeFrame);

	// Free any allocated memory.
	~FreeCamera();

	// Inherited from ICamera. Execute the necessary OpenGL methods to setup the 
	// camera's projection mode.
	virtual void SetupProjection(double w2h);

	// Inherited from ICamera. Only supports VM_PERSPECTIVE. Calls to this method 
	// have no affect.
	void SetViewMode(ViewMode viewMode);

	// Inherited from ICamera. Execute the necessary OpenGL methods to view the 
	// world from this FreeCamera.
	void Render();
};

#endif
