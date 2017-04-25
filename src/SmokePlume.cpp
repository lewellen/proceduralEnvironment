#include <algorithm>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>

#include "OpenGL.h"

#include "SmokePlume.h"

#include "IsoSurface.h"
#include "Materials.h"
#include "Metaballs.h"
#include "Random.h"
#include "Renderable.h"

SmokePlume::SmokePlume() : Renderable() {
	data = new Grid3f(16, 16, 16);

	isoSurface = IsoSurface();
	metaballs = Metaballs();
	particles = std::vector<Particle*>();

	lastAdded = clock();
}

SmokePlume::~SmokePlume() {
	delete data;

	for (std::vector<Particle*>::iterator i = particles.begin(); i != particles.end(); i++)
		delete (*i);

	particles.clear();
}

void SmokePlume::DoIdle() {
	// Update the positions of the particles based on a randomly generated 
	// vector field.
	doPhysics();

	// Clear out all the data entries.
	data->Zero();

	// Create a vector array of points to visualize.
	Vector* points = new Vector[particles.size()];
	std::vector<Particle*>::iterator i = particles.begin();
	std::vector<Particle*>::iterator end = particles.end();
	for (int k = 0; i != end; i++, k++)
		points[k] = (*i)->position;

	// Use metaballs to populate the grid with values
	metaballs.ToField(points, particles.size(), data);

	// Use an isosurface algorithm to visualize the entries.
	isoSurface.Set(data, 7);
}

void SmokePlume::Render() {
	glEnable(GL_BLEND);

	glPushMatrix();
	glTranslatef(-.5, -.5, 0);

#if _DEBUG
	glColor3f(1, 1, 1);
	glPointSize(1.0);

	glBegin(GL_POINTS);
	for (std::vector<Particle*>::iterator i = particles.begin(); i != particles.end(); i++) {
		Vector p = (*i)->position;
		glVertex3f(p.x, p.y, p.z);
	}

	glEnd();
#endif

	float ambient[] =  { 0.90, 0.90, 0.90, 1.0 };
	float diffuse[] =  { 0.90, 0.90, 0.90, 0.8 };
	float specular[] = { 1.00, 1.00, 1.00, 1.0 };
	int shininess = 28;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, shininess);

	isoSurface.Render();

	MAT_DEFAULT.Apply();

	glPopMatrix();

	glDisable(GL_BLEND);
}

void SmokePlume::doPhysics() {
	float dt = 0.01; //sec
	Vector Fg = Vector(
		2 * 9.801 * (0.5 - Random::Get()->NextFloat()),
		2 * 9.801 * (0.5 - Random::Get()->NextFloat()),
		+.9801
		); // m/s^2

	for (std::vector<Particle*>::iterator i = particles.begin(); i != particles.end();) {
		Particle* p = *i;
		Vector vOld = Vector(p->velocity);
		Vector pOld = Vector(p->position);

		// Update the position and velocity
		p->velocity = Vector(
			vOld.x + dt * Fg.x,
			vOld.y + dt * Fg.y,
			vOld.z + dt * Fg.z
			);

		p->position = Vector(
			pOld.x + vOld.x * dt + Fg.x * dt * dt * 0.5,
			pOld.y + vOld.y * dt + Fg.y * dt * dt * 0.5,
			pOld.z + vOld.z * dt + Fg.z * dt * dt * 0.5
			);

		if (
			(p->position.x <= 0 || p->position.x >= 1) ||
			(p->position.y <= 0 || p->position.y >= 1) ||
			(p->position.z <= 0 || p->position.z >= 1)
			) {
			// Particle is out of bounds so remove it
			i = particles.erase(i);
			delete p;
		}
		else {
			i++;
		}
	}

	// Add a new particle (at most) every 250ms
	clock_t now = clock();
	if (now - lastAdded > 0.25 * CLOCKS_PER_SEC) {
		for (int i = 0; i < 10; i++) {
			// Place the particle a certain distance from the origin in x
			Vector offset = Vector(.1, 0, 0);
			offset.RotateAboutZ(rand() % 360);
			offset.RotateAboutX(45 - (rand() % 90));
			offset.RotateAboutY(45 - (rand() % 90));

			// Bring the offset in or out from it's location
			float f = 0.5 - Random::Get()->NextFloat();

			// Add the point to the collection.
			Particle* p = new Particle();
			p->position = Vector(0.5 + f * offset.x, .5 + f * offset.y, .11);
			p->velocity = Vector(0, 0, 0);
			particles.push_back(p);
		}

		lastAdded = now;
	}
}