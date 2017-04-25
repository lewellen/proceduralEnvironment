#ifndef __Forest_H__
#define __Forest_H__

#include<vector>

#include "RenderableCollection.h"
#include "Terrain.h"
#include "Tree.h"

// A collection of Trees.
class Forest : public RenderableCollection {
private:
	Terrain* terrain;

protected:
	// Inherited from RenderableCollection. Positions, rotates, and scales child 
	// Renderables.
	void doGlTransforms(Renderable* item);

public:
	// Initializes a new instance with numTree Trees on the specified Terrain.
	Forest(int numTrees, Terrain* terrain);

	// Releases allocated memory for the created Trees.
	~Forest();
};

#endif
