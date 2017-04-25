#ifndef __CoordinateAxes_H__
#define __CoordinateAxes_H__

#include "Renderable.h"

// A visualization of the world coordinate axes.
class CoordinateAxes : public Renderable {
public:
	// Inherited from Renderable. Draws a unit coordinate axis.
	void Render();
};

#endif
