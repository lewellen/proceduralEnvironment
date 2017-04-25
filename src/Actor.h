#ifndef __Actor_H__
#define __Actor_H__

#include "IFrame.h"
#include "Renderable.h"

// An object in the scene that has a frame and can be rendered.
class Actor : public Renderable {
private:
	IFrame* frame;

public:
	// Initializes a new instance.
	Actor(IFrame* frame);
	
	// Releases memory associated with the instance.
	virtual ~Actor();

	// Gets the actor's frame.
	IFrame* GetFrame();

	// Sets the actor's frame.
	void SetFrame(IFrame* frame);

	// Inherited from Renderable. Renders the actor.
	virtual void Render() = 0;
};

#endif
