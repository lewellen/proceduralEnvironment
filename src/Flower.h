#ifndef __Flower_H__
#define __Flower_H__

#include "Placeable.h"
#include "Random.h"
#include "Renderable.h"
#include "RoseTree.h"

// Procedurally generated flower.
class Flower : public Placeable, public Renderable {
private:
	int depth;
	RoseTree* root;

	int barkTexture, leafTexture;
	int displayListId;

	void drawTree(RoseTree* roseTree);
	RoseTree* generate(RoseTree* parent, int numChildren, int depth);

	void drawCanopy();

	void texturedQuad(float width, float height, int texture);

public:
	// Initalizes a new flower with the given depth. Depth should be between 1 
	// and 10. Higher values create more branches factorially.
	Flower(int depth);

	// Releases memory associated with the instance.
	~Flower();

	// Inherited rom Placeable. Gets the number of rows this takes up.
	int GetRowFootPrint();

	// Inherited rom Placeable. Gets the number columns this takes up.
	int GetColumnFootPrint();

	// Inherited from Renderable. Renders the flower.
	void Render();
};

#endif
