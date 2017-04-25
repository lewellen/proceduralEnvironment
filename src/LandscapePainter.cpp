#include <cstdio>

#include <cstdio>
#include "OpenGL.h"


#include "LandscapePainter.h"

#include "Materials.h"
#include "TextureManager.h"
#include "Vector.h"

LandscapePainter::LandscapePainter() {
	whiteRock = TextureManager::Get()->Load(TEX_WHITE_ROCK);
	slate = TextureManager::Get()->Load(TEX_SLATE);
	fern = TextureManager::Get()->Load(TEX_FERN);
	moss = TextureManager::Get()->Load(TEX_MOSS);
	grass = TextureManager::Get()->Load(TEX_GRASS);
	dirt = TextureManager::Get()->Load(TEX_DIRT);
	weeds = TextureManager::Get()->Load(TEX_WEEDS);
	pebbles = TextureManager::Get()->Load(TEX_PEBBLES);
	sand = TextureManager::Get()->Load(TEX_SAND);
}

void LandscapePainter::Initialize() {
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	MAT_DEFAULT.Apply();
}

void LandscapePainter::Deinitialize() {
	glDisable(GL_TEXTURE_2D);
}

void LandscapePainter::Color(
	float a[],
	float b[],
	float c[],
	float d[],
	Vector* aNorm,
	Vector* bNorm,
	Vector* cNorm,
	Vector* dNorm
	) {

	float f = 10;
	glBindTexture(GL_TEXTURE_2D, getTexture(a[2], aNorm, &f));

	glBegin(GL_QUADS);

	putVertex(a, aNorm, f);
	putVertex(b, bNorm, f);
	putVertex(c, cNorm, f);
	putVertex(d, dNorm, f);

	glEnd();
}

void LandscapePainter::putVertex(float* v, Vector* n, float f) {
	if (n != NULL)
		glNormal3f(n->x, n->y, n->z);

	glTexCoord2f(f*v[0], f*v[1]);
	glVertex3fv(v);
}

int LandscapePainter::getTexture(float z, Vector* norm, float* scale) {
	int textureToUse = -1;

	if (norm != NULL) {
		Vector up = Vector(0, 0, 1);
		float n = up.Inner(norm);
		if (n < .5) {
			if (z > .2){
				textureToUse = slate;
				*scale = 10;
			}
			else if (z > .15) {
				textureToUse = moss;
				*scale = 16;
			}
			else if (z > .1) {
				textureToUse = moss;
				*scale = 20;
			}
			else if (z > .05) {
				textureToUse = weeds;
				*scale = 30;
			}
			else {
				textureToUse = pebbles;
				*scale = 20;
			}
		}
	}

	if (textureToUse < 0) {
		if (z > .2) {
			textureToUse = whiteRock;
			*scale = 2;
		}
		else if (z > .15) {
			textureToUse = moss;
			*scale = 8;
		}
		else if (z > .1) {
			textureToUse = grass;
			*scale = 15;
		}
		else if (z > .05) {
			textureToUse = dirt;
			*scale = 10;
		}
		else {
			textureToUse = sand;
			*scale = 30;
		}
	}

	return textureToUse;
}