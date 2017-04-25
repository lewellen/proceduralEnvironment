#include <cstdio>
#include <cmath>

#include "OpenGL.h"


#include "Water.h"

#include "Materials.h"
#include "Terrain.h"
#include "TextureManager.h"
#include "WaterShader.h"

#include "SchreuderUtil.h"

Water::Water(float elevation, Terrain* terrain) {
	this->terrain = terrain;
	this->elevation = elevation;

	waterShader = new WaterShader();
	reflectionBuffer = NULL;
	refractionBuffer = NULL;

	waterBumpTexture = TextureManager::Get()->Load(TEX_WATER_BUMP);
	plainWaterTexture = TextureManager::Get()->Load(TEX_WATER);

	time = 0;
	timeStep = 0.10;
}

Water::~Water() {
	delete waterShader;
	waterShader = NULL;

	reflectionBuffer = NULL;
	refractionBuffer = NULL;
}

void Water::DoIdle() {
	time += timeStep;
}

void Water::SetBuffers(FrameBuffer* reflection, FrameBuffer* refraction) {
	this->reflectionBuffer = reflection;
	this->refractionBuffer = refraction;
}

void Water::Render() {
	if (GetPass() != RP_FINAL)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	float c2x = 1.0 / (float)this->terrain->GetColumns();
	float r2y = 1.0 / (float)this->terrain->GetRows();

	if (reflectionBuffer != NULL) {
		waterShader->Begin();
		waterShader->SetTime(time);
		waterShader->SetMinSeaFloor(terrain->GetMin());

		waterShader->SetReflectionTexture(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, reflectionBuffer->ColorTexture());

		waterShader->SetRefractionTexture(1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, refractionBuffer->ColorTexture());

		waterShader->SetBumpTexture(2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterBumpTexture);

		waterShader->SetDepthTexture(3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, refractionBuffer->DepthTexture());

	}
	else {
		waterShader->Begin();
		waterShader->SetTime(time);
		waterShader->SetMinSeaFloor(terrain->GetMin());

		waterShader->SetReflectionTexture(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, plainWaterTexture);

		waterShader->SetBumpTexture(2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterBumpTexture);

	}

	glBegin(GL_QUADS);

	for (int row = 0; row < this->terrain->GetRows() - 1; row++) {
		for (int column = 0; column < this->terrain->GetColumns() - 1; column++) {
			float z = this->elevation;

			float a = this->terrain->Get(row, column);
			float b = this->terrain->Get(row, column + 1);
			float c = this->terrain->Get(row + 1, column + 1);
			float d = this->terrain->Get(row + 1, column);

			if (a > z && b > z && c > z && d > z)
				continue;

			a = fmin(a, z);
			b = fmin(b, z);
			c = fmin(c, z);
			d = fmin(d, z);

			float x = column * c2x;
			float y = row * r2y;

			// Surface
			glNormal3f(0, 0, 1);

			glTexCoord2f(x, y);
			setVertex(x, y, z, a, true);

			glTexCoord2f(x + c2x, y);
			setVertex(x + c2x, y, z, b, true);

			glTexCoord2f(x + c2x, y + r2y);
			setVertex(x + c2x, y + r2y, z, c, true);

			glTexCoord2f(x, y + r2y);
			setVertex(x, y + r2y, z, d, true);

			if (row == 0) {
				// Back face
				glNormal3f(0, -1, 0);

				setVertex(x, y, a, a, false);
				setVertex(x + c2x, y, b, b, false);
				setVertex(x + c2x, y, z, b, true);
				setVertex(x, y, z, a, true);
			}
			else if (row == terrain->GetRows() - 2) {
				// Front face
				glNormal3f(0, +1, 0);

				setVertex(x, y + r2y, d, d, false);
				setVertex(x, y + r2y, z, d, true);
				setVertex(x + c2x, y + r2y, z, c, true);
				setVertex(x + c2x, y + r2y, c, c, false);
			}

			if (column == 0) {
				// Left face
				glNormal3f(-1, 0, 0);

				setVertex(x, y, a, a, false);
				setVertex(x, y, z, a, true);
				setVertex(x, y + r2y, z, d, true);
				setVertex(x, y + r2y, d, d, false);
			}
			else if (column == terrain->GetColumns() - 2) {
				// Right face
				glNormal3f(+1, 0, 0);

				setVertex(x + c2x, y, b, b, false);
				setVertex(x + c2x, y + r2y, c, c, false);
				setVertex(x + c2x, y + r2y, z, c, true);
				setVertex(x + c2x, y, z, b, true);
			}
		}
	}

	glEnd();

	if (reflectionBuffer != NULL) {
		waterShader->End();
	}
	else {
		waterShader->End();
	}

	glActiveTexture(GL_TEXTURE0);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	MAT_DEFAULT.Apply();
}

void Water::setVertex(float x, float y, float z, float w, bool applyWave) {
	waterShader->SetApplyWave(applyWave ? 1.0 : 0.0);
	waterShader->SetSeaFloor(w);
	glVertex3f(x, y, z);
}