#ifndef __Renderable_H__
#define __Renderable_H__

#include "IPainter.h"

// Enumeration of possible render passes.
enum RenderPass { RP_REFLECTION, RP_REFRACTION, RP_FINAL };

// Represents a thing that can be rendered by OpenGL.
class Renderable {
private:
	IPainter* painter;
	RenderPass pass;

public:
	// Initialize member variables.
	Renderable();

	// Releases any memory associated with the instance.
	virtual ~Renderable();

	virtual void DoIdle();

	// Retrieves the underlying painter.
	IPainter* GetPainter();

	RenderPass GetPass();

	// Renders the thing by OpenGL.
	virtual void Render() = 0;

	// Sets the underlying painter.
	void SetPainter(IPainter* painter);

	virtual void SetPass(RenderPass renderPass);
};

#endif
