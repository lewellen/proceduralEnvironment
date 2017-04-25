#ifndef __Neighborhood_H__
#define __Neighborhood_H__

#include<vector>

#include "House.h"
#include "Renderable.h"
#include "RenderableCollection.h"
#include "Terrain.h"

// A collection of houses.
class Neighborhood : public RenderableCollection {
private:
	Terrain* terrain;
	House* lastHouse;

protected:
	// Inherited from RenderableCollection. Positions, rotates, and scales the 
	// child Renderables.
	void doGlTransforms(Renderable* item);

public:
	// Initializes a new instance with numHomes houses on the specified Terrain.
	Neighborhood(int numHomes, Terrain* terrain);

	// Releases allocated memory for the created Houses.
	~Neighborhood();

	// Toggles house smoke on or off.
	void ToggleSmoke();
};

#endif
