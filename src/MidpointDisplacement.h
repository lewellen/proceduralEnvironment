#ifndef __MidpointDisplacement_H__
#define __MidpointDisplacement_H__

#include "Random.h"
#include "Renderable.h"
#include "Vector.h"

#include "Grid2f.h"

// Represents the midpoint-displacement algorithm for creating fractal terrains.
class MidpointDisplacement {
private:
	void populate(Grid2f* A, int row, int column, int rows, int columns);
	void smooth(Grid2f** A);
	float nextFloat(float min, float max);
	Vector** allocNorms(int rows, int columns);

public:
	// Initializes a new instance.
	MidpointDisplacement();

	// Releases all memory associated with the instance.
	~MidpointDisplacement();

	// Creates a new two dimensional grid with the specified edgeSize, and 
	// corner seed values.
	Grid2f* Populate(int edgeSize, float a, float b, float c, float d);
	
	// Computes the normals of the given two dimensional grid.
	Vector** Norms(Grid2f* A);
};

#endif