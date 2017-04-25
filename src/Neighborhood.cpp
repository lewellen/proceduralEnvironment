#include <cstdio>
#include "OpenGL.h"

#include "Neighborhood.h"

#include "House.h"
#include "Random.h"
#include "RenderableCollection.h"
#include "Terrain.h"

Neighborhood::Neighborhood(int numHomes, Terrain* terrain) : RenderableCollection() {
	lastHouse = NULL;

	this->terrain = terrain;

	for (int t = 1; t <= numHomes; t++) {
		//int rotation = this->prng->NextInt(360);
		//float f = 0.5 * ((rotation / 360.0) + 1);

		House* house = new House(180, .5);
		this->terrain->Place(house, 0.04, 0.24);
		RenderableCollection::Items()->push_back(house);

		// hack....
		lastHouse = house;
	}
}

Neighborhood::~Neighborhood() {
	this->terrain = NULL;
}

void Neighborhood::doGlTransforms(Renderable* i) {
	House* item = (House*)i;

	float c2x = 1.0 / (float)terrain->GetColumns();
	float r2y = 1.0 / (float)terrain->GetRows();

	int row = item->GetRow();
	int column = item->GetColumn();

	float x = c2x * column;
	float y = r2y * row;
	float z = this->terrain->Get(row, column);

	int theta = item->GetRotation();
	float f = item->GetScale();

	glTranslatef(x, y, z);
	glScalef(0.2 * f, 0.2* f, 0.2* f);
	glRotatef(theta, 0, 0, 1);
	glTranslatef(-0.5, -0.5, 0);
}

void Neighborhood::ToggleSmoke() {
	if (lastHouse != NULL)
		lastHouse->ToggleSmoke();
}