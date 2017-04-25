#ifndef __Metaballs_H__
#define __Metaballs_H__

#include "Grid3f.h"
#include "Vector.h"

// Represents the metaballs algorithm. Converts a collection of points into a 
// scalar field.
class Metaballs {
public:
	// Converts the collection of points into a scalar field with the results 
	// assigned to the data instance.
	void ToField(Vector* points, int numPoints, Grid3f* data);
};

#endif
