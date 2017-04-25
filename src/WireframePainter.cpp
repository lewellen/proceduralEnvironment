#include "OpenGL.h"


#include "WireframePainter.h"

#include "Materials.h"
#include "Vector.h"

void WireframePainter::Initialize() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WireframePainter::Deinitialize() {
	glDisable(GL_BLEND);
}

void WireframePainter::Color(
	float a[],
	float b[],
	float c[],
	float d[],
	Vector* aNorm,
	Vector* bNorm,
	Vector* cNorm,
	Vector* dNorm
	) {

	// Draw the quad defined by the coordinates as a wire frame
	MAT_DEFAULT.diffuse[3] = 0.5;
	MAT_DEFAULT.Apply();
	MAT_DEFAULT.diffuse[3] = 1;

	glBegin(GL_LINE_STRIP);

	putVertex(a, aNorm);
	putVertex(b, bNorm);
	putVertex(c, cNorm);
	putVertex(d, dNorm);

	glEnd();

	// Draw the normals at each vertex
	MAT_LEAF.Apply();

	drawNorm(a, aNorm);
	drawNorm(b, bNorm);
	drawNorm(c, cNorm);
	drawNorm(d, dNorm);
}

void WireframePainter::drawNorm(float* v, Vector* norm) {
	if (norm == NULL)
		return;

	glBegin(GL_LINES);

	glVertex3f(v[0], v[1], v[2]);
	glVertex3f(
		v[0] + 0.01*norm->x,
		v[1] + 0.01*norm->y,
		v[2] + 0.01*norm->z
		);

	glEnd();
}

void WireframePainter::putVertex(float* v, Vector* n) {
	if (n != NULL)
		glNormal3f(n->x, n->y, n->z);

	glVertex3fv(v);
}