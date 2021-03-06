#include <cmath>
#include <cstdio>

#include "OpenGL.h"


#include "Flower.h"

#include "Materials.h"
#include "Placeable.h"
#include "Random.h"
#include "RoseTree.h"
#include "TextureManager.h"
#include "Terrain.h"

#define PI 3.14159265

Flower::Flower(int depth) : Placeable() {
	this->depth = depth;
	this->root = generate(new RoseTree(.1, .5, 0, 0, depth), 5, depth);

	this->barkTexture = TextureManager::Get()->Load(TEX_BARK);
	this->leafTexture = TextureManager::Get()->Load(TEX_LEAF);

	this->displayListId = glGenLists(1);

	Placeable::SetScale(0.005);

	glNewList(displayListId, GL_COMPILE);

	MAT_DEFAULT.Apply();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	*/
	glPushMatrix();

	glTranslatef(.5, .5, 0);

	drawTree(this->root);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glEndList();
}

Flower::~Flower() {
	glDeleteLists(displayListId, 1);

	delete this->root;
	this->root = NULL;
}

int Flower::GetRowFootPrint() {
	return 3;
}

int Flower::GetColumnFootPrint() {
	return 3;
}

void Flower::Render() {
	glCallList(displayListId);
}

void Flower::drawCanopy() {
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, leafTexture);

	float r = .1;			// Radius of canopy ball
	float W = 3 * 1.0;	// Width of leaf
	float L = 1.6 * 0.2;	// Length of leaf


	for (int i = 0; i < 10; i++) {
		float yaw = rand() % 360;
		float pitch = (rand() % 90) - 90;
		float roll = 15 - (rand() % 30);

		glPushMatrix();

		glRotatef(yaw, 0, 0, 1);	// swing
		glRotatef(roll, 1, 0, 0);	// wobble
		glRotatef(pitch, 0, 1, 0);	// elevate

		// Randomly place leaf between [0, r] from the origin
		glTranslatef(r * (rand() / (float)RAND_MAX), 0, 0);

		// Randomly scale the leaf between [0.5, 1] its original size.
		float s = .5 * (rand() / (float)RAND_MAX) + .5;
		glScalef(s, s, s);

		// Draw segments of the leaf.
		for (int j = 0; j < 20; j++) {
			glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);

			glTexCoord2f(0, j * 0.2);
			glVertex3f(0, -0.5 * W /* (j + 0.0) * 0.2*/, 0);

			glTexCoord2f(0, (j + 1) * 0.2);
			glVertex3f(0 + L, -0.5 * W /* (j + 1.0) * 0.2*/, 0);

			glTexCoord2f(1, (j + 1) * 0.2);
			glVertex3f(0 + L, 0.5 * W /* (j + 1.0) * 0.2*/, 0);

			glTexCoord2f(1, j * 0.2);
			glVertex3f(0, 0.5 * W /* (j + 0.0) * 0.2*/, 0);

			glEnd();

			// Offset for the next segment
			glTranslatef(L, 0, 0);

			// add a small amount of rotation for the next segment to give the 
			// leaf a drooping effect
			glRotatef(1, 0, 1, 0);
		}

		glPopMatrix();
	}

	glEnable(GL_CULL_FACE);
}

void Flower::drawTree(RoseTree* roseTree) {
	int children = roseTree->Children.size();

	glPushMatrix();

	// Rotate 2nd about the z-axis (branch direction)
	glRotatef(roseTree->GetTheta(), 0, 0, 1);

	// Rotate 1st about the y-axis (branch angle)
	glRotatef(roseTree->GetPhi(), 0, 1, 0);

	if (children == 0) {

		// Leaf needs to be drawn.
		glPushMatrix();
		glScalef(.3, .3, .3);
		
		if (roseTree->GetDepth() == 5 || Random::Get()->NextInt(0, 3) != 0) {
			MAT_LEAF.Apply();
		}
		else {
			MAT_DEFAULT.Apply();
		}

		drawCanopy();

		glPopMatrix();
	}
	else {
		// Branch needs to be drawn.
		MAT_DEFAULT.Apply();
		glBindTexture(GL_TEXTURE_2D, barkTexture);

		glBegin(GL_QUAD_STRIP);

		float r = roseTree->GetBaseRadius();
		Vector n = Vector(roseTree->GetLength(), 0, 0.5*r);

		float segments = 8;
		int dTheta = 360.0 / segments;
		for (int i = 0; i <= segments; i++) {
			float x = cos(i * dTheta / 180.0 * PI);
			float y = sin(i * dTheta / 180.0 * PI);

			n.RotateAboutZ(dTheta);
			glNormal3f(n.x, n.y, n.z);

			glTexCoord2f(x, roseTree->GetDepth());
			glVertex3f(x * r, y * r, roseTree->GetLength());

			glTexCoord2f(x, 0);
			glVertex3f(x * r, y * r, 0);
		}

		glEnd();
	}

	float p = roseTree->GetLength();
	float dp = p;// / (children + 5.0);
	if (roseTree == root)
		dp = 0;

	// Begin branches half way up the current one.
	//glTranslatef(0, 0, 0.5 * p);

	std::vector<RoseTree*>::iterator i = roseTree->Children.begin();
	std::vector<RoseTree*>::iterator end = roseTree->Children.end();
	for (; i != end; i++) {
		// Incrementally translate new branches up the length of the parent.
		glTranslatef(0, 0, dp);
		drawTree(*i);
	}

	glPopMatrix();
}

RoseTree* Flower::generate(RoseTree* parent, int numChildren, int depth) {
	if (depth == 0 || numChildren == 0)
		return parent;

	if (numChildren > 1) {
		for (int i = 0; i < numChildren; i++) {
			parent->Children.push_back(generate(
				new RoseTree(
				parent->GetBaseRadius() * 0.95,
				parent->GetLength() * 1.1,
				360.0 / numChildren * i,
				10,
				depth
				),
				1, depth - 1 - i
				));
		}
	}
	else {
		parent->Children.push_back(generate(
			new RoseTree(
			parent->GetBaseRadius() * 0.95,
			parent->GetLength() * 1.3,
			0,
			10,
			depth
			),
			1, depth - 1
			));
	}


	return parent;
}

void Flower::texturedQuad(float width, float height, int texture) {
	float segments = 7;
	float dSegment = 1.0 / segments;

	float dW = width / segments;
	float dH = height / segments;

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	float f = 1;

	for (int i = 0; i < segments; i++) {
		for (int j = 0; j < segments; j++) {
			glTexCoord2f(dSegment * i * f, dSegment * j* f);
			glVertex3f(dW * i, dH * j, 0);

			glTexCoord2f(dSegment * (i + 1)* f, dSegment * j* f);
			glVertex3f(dW * (i + 1), dH * j, 0);

			glTexCoord2f(dSegment * (i + 1)* f, dSegment * (j + 1)* f);
			glVertex3f(dW * (i + 1), dH * (j + 1), 0);

			glTexCoord2f(dSegment * i* f, dSegment * (j + 1)* f);
			glVertex3f(dW * i, dH * (j + 1), 0);
		}
	}

	glEnd();
}
