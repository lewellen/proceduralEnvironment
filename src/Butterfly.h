#ifndef __Butterfly_H__
#define __Butterfly_H__

#include "Renderable.h"
#include "Vector.h"

// Represents a butterfly that flies through the random cube chasing a Brownian 
// particle.
class Butterfly : public Renderable {
private:
	unsigned int textureHandle;

	Vector position;
	Vector target;
	Vector targetDirection;

	float butterflySpeed;
	float targetSpeed;
	int elapsedTimeSteps;

public:
	// Initializes a new instance.
	Butterfly();

	// Releases memory associated with the instance.
	~Butterfly();

	// Inherited from Renderable. Updates the butterfly's position and 
	// velocity.
	void DoIdle();

	// Inherited from Renderable. Draws the butterfly.
	void Render();
};

#endif