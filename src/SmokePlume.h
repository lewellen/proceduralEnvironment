#ifndef __SmokePlume_H__
#define __SmokePlume_H__

#include <ctime>
#include <vector>

#include "Grid3f.h"
#include "IsoSurface.h"
#include "Metaballs.h"
#include "Particle.h"
#include "Renderable.h"

// Represents a smoke plume generated by the house's chimney.
class SmokePlume : public Renderable {
private:
	Grid3f* data;

	IsoSurface isoSurface;
	Metaballs metaballs;

	std::vector<Particle*> particles;
	clock_t lastAdded;

	void doPhysics();

public:
	// Initializes a new instance.
	SmokePlume();

	// Deallocates memory associated with the instance.
	~SmokePlume();

	// Inherited from Renderable. Update's the smoke's position and form.
	void DoIdle();

	// Inherited from Renderable. Draws the smoke.
	void Render();
};

#endif