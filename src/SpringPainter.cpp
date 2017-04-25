#include "OpenGL.h"


#include "SpringPainter.h"

#include "Vector.h"

void SpringPainter::Initialize() {

}

void SpringPainter::Deinitialize() {

}

void SpringPainter::Color(
	float a[],
	float b[],
	float c[],
	float d[],
	Vector* aNorm,
	Vector* bNorm,
	Vector* cNorm,
	Vector* dNorm
	) {

	glBegin(GL_QUADS);

	putVertex(a, aNorm);
	putVertex(b, bNorm);
	putVertex(c, cNorm);
	putVertex(d, dNorm);

	glEnd();
}

void SpringPainter::putVertex(float* v, Vector* n) {
	if (n != NULL)
		glNormal3f(n->x, n->y, n->z);

	setMaterial(v[2], n);
	glVertex3fv(v);
}

void SpringPainter::setMaterial(float z, Vector* norm) {
	// x & y don't really matter; want to make use of z and norm.

	float g = (1 + z)*.5;

	float ambient[] = { z, .2*g + .05, .1, 1 };
	float diffuse[] = { z, .3*g + .05, .1, 1 };
	float specular[] = { .3, .2, .1, 1 };
	float shininess = 128;

	if (norm != NULL) {
		Vector v = Vector(0, 0, 1);
		float f = v.Inner(norm);

		if (f > .8 && z > 0.2) {
			diffuse[0] = 1 * f + diffuse[0] * (1 - f);
			diffuse[1] = 1 * f + diffuse[1] * (1 - f);
			diffuse[2] = 1 * f + diffuse[2] * (1 - f);
			shininess = 64;
		}
	}

	if (z < -.01) {
		float g = (z + .3) / .3;
		if (g < 0)
			g = 0;
		else if (g > 1)
			g = 1;

		diffuse[0] = diffuse[1] * g + 0.00 * (1 - g);
		diffuse[1] = diffuse[2] * g + 0.00 * (1 - g);
		diffuse[2] = diffuse[3] * g + 0.20 * (1 - g);
	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
