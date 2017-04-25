#ifndef __ProceduralPainter_H__
#define __ProceduralPainter_H__

#include "IPainter.h"
#include "Vector.h"
#include "Grid2f.h"

// Defines a method of coloring a surface resembling a landscape.
class ProceduralPainter : public IPainter {
private:
	unsigned int textureHandle;

	int getTexture(float z, Vector* norm, float* scale);
	void putVertex(float* v, Vector* norm, float f);

public:
	// Initializes a new instance.
	ProceduralPainter(Grid2f* array, Vector** norms, float minDepth);

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
