#include <cstdio>
#include "OpenGL.h"


#include "Forest.h"

#include "RenderableCollection.h"
#include "Terrain.h"
#include "Tree.h"
#include "Flower.h"

Forest::Forest(int numTrees, Terrain* terrain) : RenderableCollection() {
	this->terrain = terrain;

	for (int t = 1; t <= numTrees; t++) {
		int choice = Random::Get()->NextInt(2);

		//if (choice == 0) {
		//	PalmTree* PalmTree = new PalmTree();
		//	this->terrain->Place(PalmTree, 0.04, 0.15);
		//	RenderableCollection::Items()->push_back(PalmTree);
		//}
		//else {
			Tree* tree = new Tree(5);
			this->terrain->Place(tree, 0.10, 0.25);
			RenderableCollection::Items()->push_back(tree);
		//}

	}
}

Forest::~Forest() {
	// Didn't create terrain, so will not delete it.
	this->terrain = NULL;
}

void Forest::doGlTransforms(Renderable* i) {
	Tree* item = (Tree*)i;

	float c2x = 1.0 / (float)terrain->GetColumns();
	float r2y = 1.0 / (float)terrain->GetRows();

	int row = item->GetRow();
	int column = item->GetColumn();

	float x = c2x * column;
	float y = r2y * row;
	float z = this->terrain->Get(row, column);

	glTranslatef(x, y, z);

	float scale = item->GetScale();
	glScalef(scale, scale, scale);
	glTranslatef(-.5, -.5, 0);
}