#ifndef __ICamera_H__
#define __ICamera_H__

#include "Renderable.h"

// Projection type.
enum ViewMode { VM_PERSPECTIVE, VM_ORTHOGONAL };

// Interface through which all cameras should adhere to view the world.
class ICamera : public Renderable {
protected:
	ViewMode viewMode;

public:
	// This is only here so that G++ doesn't complain.
	virtual ~ICamera() {}

	// Execute the necessary OpenGL methods to setup the camera's projection mode.
	virtual void SetupProjection(double w2h) = 0;

	// Switches the camera projection mode (assuming the camera supports that mode.)
	// If it does not, then no change.
	virtual void SetViewMode(ViewMode viewMode) = 0;

	// Execute the necessary OpenGL methods to view the world from this camera.
	virtual void Render() = 0;
};

#endif