#ifndef __IsoSurface_H__
#define __IsoSurface_H__

#include "Renderable.h"
#include "Grid3f.h"

// Given a scalar field and threshold, calculates the underlying isosurface.
class IsoSurface : public Renderable {
private:
	Grid3f* data;
	float maxT;

	bool isOutside(float f);

	void renderAsCubes();
	void renderAsPointCloud();
	void renderAsMarchingTetrahedra();

	void render5Cell(int column, int row, int plane);
	void render6Cell(int column, int row, int plane);

	void tetrahedron(float* a, float aVal, float* b, float bVal, float*c, float cVal, float* d, float dVal);

	void findMidpoint(float* ab, float* a, float* b, float aVal, float bVal);
	void faceNormal(float* from, float* to, float* to2);

	void vertexNormal(float* a, float* output);
	void interpolatedNormal(float* abNorm, float* aNorm, float aVal, float* bNorm, float bVal);


#ifdef DIAGNOSTIC_MODE
	void renderCage();
	void point(float* a, bool A);
#endif

public:
	// Initializes a new instance.
	IsoSurface();

	// Sets the internal data to render.
	void Set(Grid3f* data, float maxT);

	// Inherited from Renderable. Draws the surface.
	void Render();
};

#endif