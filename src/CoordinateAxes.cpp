#include <cstdio>
#include "OpenGL.h"


#include "CoordinateAxes.h"

void CoordinateAxes::Render() {
	char labels[] = { 'x', 'y', 'z' };
	float vec[] = { 0, 0, 0 };
	float zero[] = { 0, 0, 0 };
	float one[] = { 1, 1, 1 };

	for (int i = 0; i < 3; i++) {
		vec[i] = 1.0;
		glBegin(GL_LINES);
		glColor3fv(vec);
		glVertex3fv(zero);
		glVertex3fv(vec);
		glEnd();

		vec[i] = 1.1;
		glColor3fv(one);
		glRasterPos3fv(vec);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, labels[i]);

		vec[i] = 0.0;
	}
}
