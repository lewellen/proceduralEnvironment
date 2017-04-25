#ifndef __SpringPainter_H__
#define __SpringPainter_H__

#include "IPainter.h"
#include "Vector.h"

// Defines a method of coloring a surface resembling a spring time environment.
class SpringPainter : public IPainter {
private:
	void putVertex(float* v, Vector* n);
	void setMaterial(float z, Vector* norm);

public:
	// Inherited from IPainter. Initialize whatever needs to be set for the coloring to work.
	void Initialize();

	// Inherited from IPainter. Unset whatever was done to make coloring work.
	void Deinitialize();

	// Inherited from IPainter. Execute the necessary OpenGL methods to color a set of vertices.
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
