#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include "IsoSurface.h"

#include "Primitives.h"

#include "Grid3f.h"

IsoSurface::IsoSurface() {
	data = NULL;
};

void IsoSurface::Render() {
	if (data == NULL)
		return;

#ifdef DIAGNOSTIC_MODE
	renderCage();
#endif

	glPushMatrix();

	glScalef(1.0 / data->Columns(), 1.0 / data->Rows(), 1.0 / data->Planes());

	//renderAsPointCloud();
	//renderAsCubes();
	renderAsMarchingTetrahedra();

	glPopMatrix();
}

void IsoSurface::Set(Grid3f* data, float maxT) {
	this->data = data;
	this->maxT = maxT;
}

void IsoSurface::render6Cell(int row, int column, int plane) {
	// 2015-06-28 GEL Decided to change the segmentation of the cube into 6 
	// cells rather than 5 since the edges between adjacent cells in the 5 cell 
	// case do not align perfectly all the time. Segmentation given by 
	// http://paulbourke.net/geometry/polygonise/ and 
	// https://en.wikipedia.org/wiki/Marching_tetrahedra

	float a[] = { column + 0, row + 0, plane + 0 };
	float b[] = { column + 1, row + 0, plane + 0 };
	float c[] = { column + 1, row + 1, plane + 0 };
	float d[] = { column + 0, row + 1, plane + 0 };
	float e[] = { column + 0, row + 0, plane + 1 };
	float f[] = { column + 1, row + 0, plane + 1 };
	float g[] = { column + 1, row + 1, plane + 1 };
	float h[] = { column + 0, row + 1, plane + 1 };

	float aVal = data->Get(column + 0, row + 0, plane + 0);
	float bVal = data->Get(column + 1, row + 0, plane + 0);
	float cVal = data->Get(column + 1, row + 1, plane + 0);
	float dVal = data->Get(column + 0, row + 1, plane + 0);
	float eVal = data->Get(column + 0, row + 0, plane + 1);
	float fVal = data->Get(column + 1, row + 0, plane + 1);
	float gVal = data->Get(column + 1, row + 1, plane + 1);
	float hVal = data->Get(column + 0, row + 1, plane + 1);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 0, 0);
#endif

	tetrahedron(
		f, fVal,
		g, gVal,
		b, bVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(0, 1, 0);
#endif

	tetrahedron(
		c, cVal,
		b, bVal,
		g, gVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 1, 0);
#endif

	tetrahedron(
		d, dVal,
		c, cVal,
		g, gVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(0, 0, 1);
#endif

	tetrahedron(
		d, dVal,
		g, gVal,
		h, hVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 0, 1);
#endif

	tetrahedron(
		e, eVal,
		g, gVal,
		f, fVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(0, 1, 1);
#endif

	tetrahedron(
		e, eVal,
		h, hVal,
		g, gVal,
		a, aVal
		);
}

void IsoSurface::render5Cell(int row, int column, int plane) {
	float a[] = { column + 0, row + 0, plane + 0 };
	float b[] = { column + 1, row + 0, plane + 0 };
	float c[] = { column + 1, row + 1, plane + 0 };
	float d[] = { column + 0, row + 1, plane + 0 };
	float e[] = { column + 0, row + 0, plane + 1 };
	float f[] = { column + 1, row + 0, plane + 1 };
	float g[] = { column + 1, row + 1, plane + 1 };
	float h[] = { column + 0, row + 1, plane + 1 };

	float aVal = data->Get(column + 0, row + 0, plane + 0);
	float bVal = data->Get(column + 1, row + 0, plane + 0);
	float cVal = data->Get(column + 1, row + 1, plane + 0);
	float dVal = data->Get(column + 0, row + 1, plane + 0);
	float eVal = data->Get(column + 0, row + 0, plane + 1);
	float fVal = data->Get(column + 1, row + 0, plane + 1);
	float gVal = data->Get(column + 1, row + 1, plane + 1);
	float hVal = data->Get(column + 0, row + 1, plane + 1);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 0, 0);
#endif

	// (TESTED) [1] (0, 0, 0) Corner
	tetrahedron(
		e, eVal,
		d, dVal,
		b, bVal,
		a, aVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(0, 1, 0);
#endif

	// (TESTED) [2] (1, 1, 0) Corner
	tetrahedron(
		g, gVal,
		b, bVal,
		d, dVal,
		c, cVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 1, 0);
#endif

	// (TESTED) [3] (1, 0, 1) Corner
	tetrahedron(
		b, bVal,
		g, gVal,
		e, eVal,
		f, fVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(0, 0, 1);
#endif

	// [4] (0, 1, 1) Corner
	tetrahedron(
		d, dVal,
		e, eVal,
		g, gVal,
		h, hVal
		);

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 0, 1);
#endif

	// [5] Interior
	tetrahedron(
		e, eVal,
		b, bVal,
		d, dVal,
		g, gVal
		);
}

void IsoSurface::findMidpoint(float* ab, float* a, float* b, float aVal, float bVal) {
	if (aVal > maxT && bVal < maxT) {
		// a outside; b inside
		float alpha = (maxT - bVal) / (aVal - bVal);
		ab[0] = b[0] + alpha * (a[0] - b[0]);
		ab[1] = b[1] + alpha * (a[1] - b[1]);
		ab[2] = b[2] + alpha * (a[2] - b[2]);
	}
	else if (aVal < maxT && bVal > maxT) {
		// b inside; a outside;
		float alpha = (maxT - aVal) / (bVal - aVal);
		ab[0] = a[0] + alpha * (b[0] - a[0]);
		ab[1] = a[1] + alpha * (b[1] - a[1]);
		ab[2] = a[2] + alpha * (b[2] - a[2]);
	}
	else {
		 //Either both outside or both inside.
		ab[0] = 0.5 * (a[0] + b[0]);
		ab[1] = 0.5 * (a[1] + b[1]);
		ab[2] = 0.5 * (a[2] + b[2]);
	}

	ab[0] = roundf(ab[0] * 100.0) / 100.0;
	ab[1] = roundf(ab[1] * 100.0) / 100.0;
	ab[2] = roundf(ab[2] * 100.0) / 100.0;
}

#ifdef DIAGNOSTIC_MODE
void IsoSurface::point(float* a, bool A) {
	if (A) {
		// outside
		glRasterPos3fv(a);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'O');
	}
	else {
		// inside
		glRasterPos3fv(a);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'I');
	}

	glPointSize(5.0);

	glBegin(GL_POINTS);
	glVertex3fv(a);
	glEnd();
}
#endif

void IsoSurface::faceNormal(float* from, float* to, float* to2) {
#ifdef DIAGNOSTIC_MODE
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);

	float p[3] = {
		(from[0] + to[0] + to2[0] ) * 0.333333,
		(from[1] + to[1] + to2[1] ) * 0.333333,
		(from[2] + to[2] + to2[2] ) * 0.333333,
	};
#endif

	Vector a = Vector(to[0] - from[0], to[1] - from[1], to[2] - from[2]);
	Vector b = Vector(to2[0] - from[0], to2[1] - from[1], to2[2] - from[2]);
	Vector* c = a.Cross(&b)->Normalize();

#ifdef DIAGNOSTIC_MODE
	glColor3f(1, 1, 1);
	glLineWidth(2.0);

	glBegin(GL_LINES);

	glVertex3fv(p);
	glVertex3f(
		p[0] + 0.05 * c->x,
		p[1] + 0.05 * c->y,
		p[2] + 0.05 * c->z
		);

	glEnd();
#endif

	glNormal3f(c->x, c->y, c->z);

	delete c;

#ifdef DIAGNOSTIC_MODE
	glColor4fv(currentColor);
#endif
}

void IsoSurface::vertexNormal(float* a, float* output) {
	int column = a[0], 
		row = a[1], 
		plane = a[2];

	float dx = 0.0;
	if (column - 1 < 0 || column + 1 >= data->Columns()) {
		if (column - 1 < 0 && column + 1 >= data->Columns()) {
			dx = 0;
		}
		else if (column - 1 < 0) {
			// forward difference
			dx = data->Get(column + 1, row, plane) - data->Get(column, row, plane);
		}
		else {
			// backward difference
			dx = data->Get(column, row, plane) - data->Get(column - 1, row, plane);
		}
	}
	else {
		// central difference
		dx = 0.5 * data->Get(column + 1, row, plane) - data->Get(column - 1, row, plane);
	}

	float dy = 0;
	if (row - 1 < 0 || row + 1 >= data->Rows()) {
		if (row - 1 < 0 && row + 1 >= data->Rows()) {
			dy = 0;
		}
		else if (row - 1 < 0) {
			// forward difference
			dy = data->Get(column, row + 1, plane) - data->Get(column, row, plane);
		}
		else {
			// backward difference
			dy = data->Get(column, row, plane) - data->Get(column, row - 1, plane);
		}
	}
	else {
		// central difference
		dy = 0.5 * data->Get(column, row + 1, plane) - data->Get(column, row - 1, plane);
	}

	float dz = 0;
	if (plane - 1 < 0 || plane + 1 >= data->Planes()) {
		if (plane - 1 < 0 && plane + 1 >= data->Planes()) {
			dz = 0;
		}
		else if (plane - 1 < 0) {
			// forward difference
			dz = data->Get(column, row, plane + 1) - data->Get(column, row, plane);
		}
		else {
			// backward difference
			dz = data->Get(column, row, plane) - data->Get(column, row, plane - 1);
		}
	}
	else {
		// central difference
		dz = 0.5 * data->Get(column, row, plane + 1) - data->Get(column, row, plane - 1);
	}

	float l = sqrtf(dx * dx + dy * dy + dz * dz);
	dx /= l;
	dy /= l;
	dz /= l;

	output[0] = dx;
	output[1] = dy;
	output[2] = dz;
}

void IsoSurface::interpolatedNormal(float* abNorm, float* aNorm, float aVal, float* bNorm, float bVal) {
	if (aVal > maxT && bVal < maxT) {
		// a outside; b inside
		float alpha = (maxT - bVal) / (aVal - bVal);
		abNorm[0] = bNorm[0] + alpha * (aNorm[0] - bNorm[0]);
		abNorm[1] = bNorm[1] + alpha * (aNorm[1] - bNorm[1]);
		abNorm[2] = bNorm[2] + alpha * (aNorm[2] - bNorm[2]);
	}
	else if (aVal < maxT && bVal > maxT) {
		// b inside; a outside;
		float alpha = (maxT - aVal) / (bVal - aVal);
		abNorm[0] = aNorm[0] + alpha * (bNorm[0] - aNorm[0]);
		abNorm[1] = aNorm[1] + alpha * (bNorm[1] - aNorm[1]);
		abNorm[2] = aNorm[2] + alpha * (bNorm[2] - aNorm[2]);
	}
	else {
		//Either both outside or both inside.
		abNorm[0] = 0.5 * (aNorm[0] + bNorm[0]);
		abNorm[1] = 0.5 * (aNorm[1] + bNorm[1]);
		abNorm[2] = 0.5 * (aNorm[2] + bNorm[2]);
	}

	float len = sqrtf(abNorm[0] * abNorm[0] +
		abNorm[1] * abNorm[1] +
		abNorm[2] * abNorm[2]);

	abNorm[0] /= -len;
	abNorm[1] /= -len;
	abNorm[2] /= -len;
}

void IsoSurface::tetrahedron(
	float* a, float aVal,
	float* b, float bVal,
	float* c, float cVal,
	float* d, float dVal
	) {

	bool
		A = isOutside(aVal),
		B = isOutside(bVal),
		C = isOutside(cVal),
		D = isOutside(dVal);

	// D:8 C:4 B:2 A:1
	int key = (D << 3) | (C << 2) | (B << 1) | A;

	if (key == 0 || key == 15)
		// (TESTED) All inside, or all outside. Eitherway, nothing to do.
		return;

#ifdef DIAGNOSTIC_MODE
	point(a, A);
	point(b, B);
	point(c, C);
	point(d, D);
#endif

	float ab[3]; findMidpoint(ab, a, b, aVal, bVal);
	float ac[3]; findMidpoint(ac, a, c, aVal, cVal);
	float ad[3]; findMidpoint(ad, a, d, aVal, dVal);
	float bc[3]; findMidpoint(bc, b, c, bVal, cVal);
	float bd[3]; findMidpoint(bd, b, d, bVal, dVal);
	float cd[3]; findMidpoint(cd, c, d, cVal, dVal);

	float aNorm[3]; vertexNormal(a, aNorm);
	float bNorm[3]; vertexNormal(b, bNorm);
	float cNorm[3]; vertexNormal(c, cNorm);
	float dNorm[3]; vertexNormal(d, dNorm);

	float abNorm[3]; interpolatedNormal(abNorm, aNorm, aVal, bNorm, bVal);
	float acNorm[3]; interpolatedNormal(acNorm, aNorm, aVal, cNorm, cVal);
	float adNorm[3]; interpolatedNormal(adNorm, aNorm, aVal, dNorm, dVal);
	float bcNorm[3]; interpolatedNormal(bcNorm, bNorm, bVal, cNorm, cVal);
	float bdNorm[3]; interpolatedNormal(bdNorm, bNorm, bVal, dNorm, dVal);
	float cdNorm[3]; interpolatedNormal(cdNorm, cNorm, cVal, dNorm, dVal);

	if (key == 1) {
		// (TESTED 1 2 3) A outside;  B, C, D inside
		//faceNormal(ac, ab, ad);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glEnd();
	}
	else if (key == 2) {
		// (TESTED 1 2 3) B outside;  A, C, D inside
		//faceNormal(ab, bc, bd);

		glBegin(GL_TRIANGLES);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glEnd();
	}
	else if (key == 3) {
		// (TESTED 1 2 3) A, B outside;  C, D inside
		//faceNormal(ac, bc, ad);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(adNorm); glVertex3fv(ad);

		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glEnd();
	}
	else if (key == 4) {
		// (TESTED 1 2 3) C outside; A, B, D inside
		//faceNormal(cd, bc, ac);

		glBegin(GL_TRIANGLES);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glEnd();
	}
	else if (key == 5) {
		// (TESTED 1 2 3) C, A outside; B, D inside
		//faceNormal(ad, bc, ab);

		glBegin(GL_TRIANGLES);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(abNorm); glVertex3fv(ab);

		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glEnd();
	}
	else if (key == 6) {
		// (TESTED 1 2 3) C, B outside: A, D inside
		//faceNormal(ac, bd, ab);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(abNorm); glVertex3fv(ab);

		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glEnd();
	}
	else if (key == 7) {
		// (TESTED 1 2 3) A, B, C outside: D inside
		//faceNormal(cd, bd, ad);

		glBegin(GL_TRIANGLES);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glEnd();
	}
	else if (key == 8) {
		// (TESTED 1 2 3) D outside; A, B, C inside
		//faceNormal(ad, bd, cd);

		glBegin(GL_TRIANGLES);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glEnd();
	}
	else if (key == 9) {
		// (TESTED 1 2 3) D, A outside; B, C inside
		//faceNormal(ac, ab, bd);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glNormal3fv(bdNorm); glVertex3fv(bd);

		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glEnd();
	}
	else if (key == 10) {
		// (TESTED 1 2 3) D, B outside; A, C inside
		//faceNormal(ad, ab, bc);

		glBegin(GL_TRIANGLES);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glNormal3fv(bcNorm); glVertex3fv(bc);

		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glEnd();
	}
	else if (key == 11) {
		// (TESTED 1 2 3) D, B, A outside; C inside
		//faceNormal(ac, bc, cd);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glNormal3fv(cdNorm); glVertex3fv(cd);
		glEnd();
	}
	else if (key == 12) {
		//faceNormal(ac, ad, bd);

		// (TESTED 1 2 3) D, C outside, A, B inside
		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(bdNorm); glVertex3fv(bd);

		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glEnd();
	}
	else if (key == 13) {
		// (TESTED 1 2 ) D, C, A outside; B inside
		//faceNormal(ab, bd, bc);

		glBegin(GL_TRIANGLES);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glNormal3fv(bdNorm); glVertex3fv(bd);
		glNormal3fv(bcNorm); glVertex3fv(bc);
		glEnd();
	}
	else if (key == 14) {
		// (TESTED 1 2 3) D, C, B outside; A inside
		//faceNormal(ac, ad, ab);

		glBegin(GL_TRIANGLES);
		glNormal3fv(acNorm); glVertex3fv(ac);
		glNormal3fv(adNorm); glVertex3fv(ad);
		glNormal3fv(abNorm); glVertex3fv(ab);
		glEnd();
	}
}

bool IsoSurface::isOutside(float x) {
	return x < maxT;
}

#ifdef DIAGNOSTIC_MODE
void IsoSurface::renderCage() {
	glColor3f(1, 1, 1);

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);

	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);

	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glEnd();
}
#endif

void IsoSurface::renderAsMarchingTetrahedra() {
#ifdef DIAGNOSTIC_MODE
	glEnable(GL_COLOR_MATERIAL);
#endif

	for (int column = 0; column < data->Columns() - 1; column++)
		for (int row = 0; row < data->Rows() - 1; row++)
			for (int plane = 0; plane < data->Planes() - 1; plane++)
				render6Cell(column, row, plane);

#ifdef DIAGNOSTIC_MODE
	//glDisable(GL_COLOR_MATERIAL);
#endif
}

void IsoSurface::renderAsCubes() {
	for (int column = 0; column < data->Columns() - 1; column++) {
		for (int row = 0; row < data->Rows() - 1; row++) {
			for (int plane = 0; plane < data->Planes() - 1; plane++) {

				float C[] = {
					  data->Get( column, row, plane )
					, data->Get( column, row, plane + 1 )
					, data->Get( column, row + 1, plane ) 
					, data->Get( column, row + 1, plane + 1 )
					, data->Get( column + 1, row, plane )
					, data->Get( column + 1, row, plane + 1 )
					, data->Get( column + 1, row + 1, plane )
					, data->Get( column + 1, row + 1, plane + 1 )
				};

				int key = 0;
				for (int i = 0; i < 8; i++) {
					if (!isOutside(C[i]))
						key |= 1;
					key <<= 1;
				}

				if (key == 0)
					continue;

				float x = column / (float)data->Columns();
				float y = row / (float)data->Rows();
				float z = plane / (float)data->Planes();


				glPushMatrix();

				glTranslatef(column, row, plane);
				glColor3f(x, y, z);
				cube(1.0, 1.0, 1.0, false);

				glPopMatrix();
			}
		}
	}
}

void IsoSurface::renderAsPointCloud() {  
	glBegin(GL_POINTS);

	for (int column = 0; column < data->Columns() - 1; column++) {
		for (int row = 0; row < data->Rows() - 1; row++) {
			for (int plane = 0; plane < data->Planes() - 1; plane++) {
				float fxyz = data->Get(column, row, plane);
				if (isOutside(fxyz))
					continue;

				float x = column / (float)data->Columns();
				float y = row / (float)data->Rows();
				float z = plane / (float)data->Planes();
				
				glColor4f(x, y, z, .9);
				glVertex3f(column, row, plane);
			}
		}
	}

	glEnd();
}