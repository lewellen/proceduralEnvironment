#ifndef __PalmTree_H__
#define __PalmTree_H__

#include "Placeable.h"
#include "Random.h"
#include "Renderable.h"
#include "RoseTree.h"

// Procedurally generated palm tree.
class PalmTree : public Placeable, public Renderable {
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
	// Initalizes a new palm tree with the given depth. Depth should be between 1 
	// and 10. Higher values create more branches factorially.
	PalmTree(int depth);

	// Releases memory associated with the instance.
	~PalmTree();

	// Inherited rom Placeable. Gets the number of rows this takes up.
	int GetRowFootPrint();

	// Inherited rom Placeable. Gets the number columns this takes up.
	int GetColumnFootPrint();

	// Inherited from Renderable. Renders the palm tree.
	void Render();
};

#endif
