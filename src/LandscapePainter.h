#ifndef __LandscapePainter_H__
#define __LandscapePainter_H__

#include "IPainter.h"
#include "Vector.h"

// Defines a method of coloring a surface resembling a landscape.
class LandscapePainter : public IPainter {
private:
	int whiteRock, slate, fern, moss, grass, dirt, weeds, pebbles, sand;

	int getTexture(float z, Vector* norm, float* scale);
	void putVertex(float* v, Vector* norm, float f);

public:
	// Initializes a new instance.
	LandscapePainter();

	// Initialize whatever needs to be set for the coloring to work.
	void Initialize();

	// Unset whatever was done to make coloring work.
	void Deinitialize();

	// Execute the necessary OpenGL methods to color a set of vertices.
	void Color(float x, float y, float z, Vector* norm);

	void Color(
		float a[],
		float b[],
		float c[],
		float d[],
		Vector* aNorm,
		Vector* bNorm,
		Vector* cNorm,
		Vector* dNorm
		);
};

#endif
