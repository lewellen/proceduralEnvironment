#ifndef __Terrain_H__
#define __Terrain_H__

#include "Placeable.h"
#include "Random.h"
#include "Renderable.h"
#include "Vector.h"

#include "Grid2f.h"

// Procedurally generated landscape.
class Terrain : public Renderable {
private:
	Grid2f* array;
	Vector** norms;

	float min, max;
	int displayListId;

	void freeNorms(Vector** A);
	void render();

public:
	// Initializes a new instance. Edge size should be between 1 and 10. High 
	// values generate more tiles to render which would result in slower 
	// runtime.
	Terrain(int edgeSize);

	// Releases memory associated with the instance.
	~Terrain();

	// Gets the height z at the given row and column.
	float Get(int row, int column);

	// Gets the total number of columns.
	int GetColumns();

	// Gets the maximum height z.
	float GetMax();

	// Gets the minimum height z.
	float GetMin();

	// Gets the total number of rows.
	int GetRows();

	// Randomly positions the Placeable somewhere on the terrain where the 
	// elevation is between lowerLimit and upperLimit.
	void Place(Placeable* placeable, float lowerLimit, float upperLimit);

	// Inherited from Renderable. Draws the landscape.
	void Render();
};

#endif
