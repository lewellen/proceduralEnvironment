#ifndef __RenderableCollection_H__
#define __RenderableCollection_H__

#include<vector>

#include "Renderable.h"

// A collection of Renderables.
class RenderableCollection : public Renderable {
private:
	std::vector<Renderable*>* items;

protected:
	// Removes all the items from the collection and releases their memory.
	void FreeCollection();

	// The collection of Renderables.
	std::vector<Renderable*>* Items();

	// Positions, rotates, and scales the child renderable.
	virtual void doGlTransforms(Renderable* renderable);

public:
	// Initializes a new empty instance.
	RenderableCollection();

	// Releases the memory associated with instance. RenderableCollection will 
	// free all instances in the collection.
	~RenderableCollection();

	void DoIdle();

	// Renders the collection of Renderables.
	virtual void Render();

	void SetPass(RenderPass pass);
};

#endif
