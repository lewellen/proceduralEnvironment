#ifndef __IPainter_H__
#define __IPainter_H__

#include "Vector.h"

// Defines a method of coloring a surface.
class IPainter {
public:
	// This is only here so that G++ doesn't complain.
	virtual ~IPainter() { }

	// Initialize whatever needs to be set for the coloring to work.
	virtual void Initialize() = 0;

	// Unset whatever was done to make coloring work.
	virtual void Deinitialize() = 0;

	// Execute the necessary OpenGL methods to color a set of vertices.
	virtual void Color(
		float a[],
		float b[],
		float c[],
		float d[],
		Vector* norm
		) {
		Color(a, b, c, d, norm, norm, norm, norm);
	};

	// Execute the necessary OpenGL methods to color a set of vertices.
	virtual void Color(
		float a[],
		float b[],
		float c[],
		float d[],
		Vector* aNorm,
		Vector* bNorm,
		Vector* cNorm,
		Vector* dNorm
		) = 0;
};

#endif
