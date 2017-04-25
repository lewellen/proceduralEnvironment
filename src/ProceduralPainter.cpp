#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "OpenGL.h"

#include "ProceduralPainter.h"

#include "Grid2f.h"
#include "Materials.h"
#include "MidpointDisplacement.h"
#include "SchreuderUtil.h"
#include "TextureManager.h"
#include "Vector.h"

ProceduralPainter::ProceduralPainter(Grid2f* array, Vector** norms, float lowestPoint) {
	int resultWidth = 512;
	int resultHeight = 512;
	unsigned char* resultRGBA = (unsigned char*)malloc(resultWidth * resultHeight * sizeof(unsigned char) * 4);
	memset(resultRGBA, 0, resultWidth * resultHeight * 4 * sizeof(unsigned char));

	unsigned int baseWidth = 0, baseHeight = 0;
	unsigned char* baseRGB = ReadBMP("res/terrain/sand.bmp", &baseWidth, &baseHeight);

	for (int x = 0; x < baseWidth; x++)
		for (int y = 0; y < baseHeight; y++)
			for (int i = 0; i < 3; i++)
				resultRGBA[x * resultWidth * 4 + y * 4 + i] = baseRGB[x * baseWidth * 3 + y * 3 + i];

	free(baseRGB);

	const char* files[] = {
		"res/terrain/pebbles.bmp",
		"res/terrain/dirt.bmp",
		"res/terrain/moss.bmp",
		"res/terrain/grass.bmp",
		"res/terrain/fern.bmp",
		"res/terrain/slate.bmp",
		"res/terrain/white-rock.bmp"
	};

	bool enforceNormals[] = {
		false, false, false, true, true, false, true
	};

	float minElevation[] = {
		-0.05, 0.05, 0.10, 0.15, 0.20, 0.25, .30
	};

	float maxElevation[] = {
		0.05, 0.10, 0.15, .20, .25, .30, .35
	};

	MidpointDisplacement* md = new MidpointDisplacement();
	for (int i = 0; i < 7; i++) {
		Grid2f* layerMask = md->Populate(7,
			0.5 - (rand() / (float)RAND_MAX),
			0.5 - (rand() / (float)RAND_MAX),
			0.5 - (rand() / (float)RAND_MAX),
			0.5 - (rand() / (float)RAND_MAX)
			);

		unsigned int layerWidth = 0, layerHeight = 0;
		unsigned char* layerRGB = ReadBMP(files[i], &layerWidth, &layerHeight);

		for (int x = 0; x < baseWidth; x++) {
			for (int y = 0; y < baseHeight; y++) {
				int row = y / 512.0 * 129.0;
				int column = x / 512.0 * 129.0;

				// Convert elevation to percentage [0, 1] between min and max.
				float f = array->Get(column, row);
				if (f < minElevation[i])
					f = minElevation[i];
				if (f > maxElevation[i])
					f = maxElevation[i];
				f =  (f - minElevation[i]) / (maxElevation[i] - minElevation[i]);

				// Add in noise
				f += layerMask->Get(row, column);

				// Threshold
				f = 1.0 / (1.0 + exp( -5 * (0.5 -  f)  ));

				// Decided against doing normal effects. Just doesn't look quite right.
				//if (enforceNormals[i]) {
				//	Vector up = Vector(.1, .1, 1);
				//	f *= up.Inner(&norms[column][row]);
				//}

				// Mix old texture and new texture by channel
				for (int i = 0; i < 3; i++) {
					float rX = resultRGBA[x * resultWidth * 4 + y * 4 + i];
					float lX = layerRGB[x * baseWidth * 3 + y * 3 + i];

					rX = rX * f + lX * (1 - f);
					if (rX > 255)
						rX = 255;
					if (rX < 0)
						rX = 0;

					resultRGBA[x * resultWidth * 4 + y * 4 + i] = (unsigned char)rX;
				}
			}
		}

		free(layerRGB);
	}

	delete md;

	// Do a little pre processing for the water step by darkening the terrain 
	// below zero so that lower points are darker.
	for (int x = 0; x < baseWidth; x++)
		for (int y = 0; y < baseHeight; y++) {
			float f = array->Get(x / 512.0 * 129, y / 512.0 * 129);
			if (f > 0)
				continue;

			for (int i = 0; i < 3; i++) {
				resultRGBA[x * resultWidth * 4 + y * 4 + i] *= 1.0 - (0 - f) / (0 - lowestPoint);
			}
		}

	//  Generate 2D texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//  Copy image
	glTexImage2D(GL_TEXTURE_2D, 0, 3, resultWidth, resultHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, resultRGBA);
	if (glGetError())
		Fatal("Error in glTexImage2D %s %dx%d\n", "NA", resultWidth, resultHeight);

	//  Scale linearly when image size doesn't match
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//  Free image memory
	free(resultRGBA);

	//  Return texture name
	textureHandle = texture;
}

void ProceduralPainter::Initialize() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	MAT_DEFAULT.Apply();
}

void ProceduralPainter::Deinitialize() {
	glDisable(GL_TEXTURE_2D);
}

void ProceduralPainter::Color(
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

	glBegin(GL_QUADS);

	putVertex(a, aNorm, f);
	putVertex(b, bNorm, f);
	putVertex(c, cNorm, f);
	putVertex(d, dNorm, f);

	glEnd();
}

void ProceduralPainter::putVertex(float* v, Vector* n, float f) {
	if (n != NULL)
		glNormal3f(n->x, n->y, n->z);

	glTexCoord2f(v[0], v[1]);
	glVertex3fv(v);
}