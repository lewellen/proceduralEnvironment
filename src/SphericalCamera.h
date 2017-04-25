#ifndef __SphericalCamera_H__
#define __SphericalCamera_H__

#include "ICamera.h"
#include "SphericalFrame.h"

// A camera that views the world from the interior surface of a sphere.
class SphericalCamera : public ICamera {
private:
	SphericalFrame* frame;

public:
	// Initializes a new instance with sphere radius r.
	SphericalCamera(SphericalFrame* frame, ViewMode viewMode);

	// Release any allocated memory.
	~SphericalCamera();

	// Inherited from ICamera. Modifies the properties of this camera based on key 
	// input from the user.
	void HandleKey(unsigned char key);

	// Inherited from ICamera. Modifies the properties of this camera based on 
	// special key input from the user.
	void HandleSpecialKey(int key);

	// Inherited from ICamera. Execute the necessary OpenGL methods to setup the 
	// camera's projection mode.
	void SetupProjection(double w2h);

	// Inherited from ICamera. Switches the view mode.
	virtual void SetViewMode(ViewMode viewMode);

	// Inherited from ICamera. Execute the necessary OpenGL methods to view the 
	// world from this camera.
	void Render();
};

#endif