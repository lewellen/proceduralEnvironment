#ifndef __Particle_H__
#define __Particle_H__

#include "Vector.h"

// Represents a particle having both position and velocity.
class Particle {
public:
	// The particle's position in space.
	Vector position;

	// The particle's velocity.
	Vector velocity;

	// Initializes a new instance.
	Particle();
};

#endif
