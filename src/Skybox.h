#ifndef __Skybox_H__
#define __Skybox_H__

#include "Renderable.h"

// Simulates the appearance of the sky.
class Skybox : public Renderable {
private:
	unsigned int texture;

public:
	// Initializes a new instance.
	Skybox();
	
	// Releases memory associated with the instance.
	~Skybox();

	// Inherited from Renderable. Renders the actor.
	void Render();
};

#endif
