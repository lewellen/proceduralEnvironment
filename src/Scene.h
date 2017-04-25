#ifndef __Scene_H__
#define __Scene_H__

#include "Neighborhood.h"
#include "RenderableCollection.h"
#include "Water.h"

// The top-most renderable that represents the world to render.
class Scene : public RenderableCollection {
private:
	Water* water;
	Neighborhood* neighborhood;

public:
	// Initializes a new instance. Defaults to SCENE_LANDSCAPED.
	Scene();

	// Releases memory associated with the instance.
	~Scene();

	// Gets the water instance.
	Water* GetWater();

	// Turns the house's smoke on or off.
	void ToggleSmoke();
};

#endif
