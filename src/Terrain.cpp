#include <cstdio>
#include <cmath>

#include "OpenGL.h"

#include <stdexcept>

#include "Terrain.h"

#include "Random.h"

#include "MidpointDisplacement.h"
#include "ProceduralPainter.h"

Terrain::Terrain(int edgeSize) {
	MidpointDisplacement md;
	this->array = md.Populate(edgeSize, -.25, .5, .25, -.125);
	this->norms = md.Norms(this->array);

	this->min = INFINITY;
	this->max = INFINITY;
	for (int row = 0; row < array->Rows(); row++) {
		for (int column = 0; column < array->Columns(); column++) {
			float f = array->Get(row, column);

			if (f < min)
				min = f;
			if (f > max)
				max = f;
		}
	}

	this->SetPainter(new ProceduralPainter(array, norms, min));

	displayListId = glGenLists(1);
	glNewList(displayListId, GL_COMPILE);

	SetPass(RP_FINAL);
	render();

	glEndList();
}

Terrain::~Terrain() {
	// Let go of the terrain display list
	glDeleteLists(displayListId, 1);

	// Release memory for normals
	freeNorms(this->norms);
	norms = NULL;

	// Release the array
	delete array;
	array = NULL;
}

float Terrain::Get(int row, int column) {
	return this->array->Get(row, column);
}

int Terrain::GetColumns() {
	return this->array->Columns();
}

float Terrain::GetMax() {
	return this->max;
}

float Terrain::GetMin() {
	return this->min;
}

int Terrain::GetRows() {
	return this->array->Rows();
}

void Terrain::Place(Placeable* placeable, float lowerLimit, float upperLimit) {
	Vector zP = Vector(0, 0, 1);
	int I = placeable->GetRowFootPrint(), J = placeable->GetColumnFootPrint();
	int i = 0, j = 0;

	// Find a spot to place the object.
	do {
		i = Random::Get()->NextInt(GetRows());
		j = Random::Get()->NextInt(GetColumns());

	} while (
		Get(i, j) < lowerLimit || Get(i, j) > upperLimit	// Within elevation band
		|| zP.Inner(&norms[i][j]) < .8						// On flat area
		|| i <I || i > array->Rows() - I || j < J || j > array->Columns() - I	// Within footprint bounds
		);
	
	// Put it there.
	placeable->Set(i, j);

	// Make the area around the object flat so that object isn't floating in 
	// air.
	float z = Get(i, j);
	int window = fmax(I, J);
	for (int u = -window; u <= window; u++) {
		if (i + u < 0 || i + u >= array->Columns())
			continue;

		for (int v = -window; v <= window; v++) {
			if (j + v < 0 || j + v >= array->Columns())
				continue;

			array->Set(i + u, j + v, z);
		}
	}
}

void Terrain::Render() {
	glCallList(displayListId);
}

void Terrain::freeNorms(Vector** A) {
	if (A == NULL)
		return;

	for (int i = 0; i < GetRows(); i++)
		delete[] A[i];

	delete[] A;
}

void Terrain::render() {
	// 2015-06-10 GEL Tried both GL_QUADS and GL_TRIANGLES and decided to stick 
	// with quads since it's faster and visually indistinguishable from 
	// triangles even though the approach isn't 100% correct. (Triangles would 
	// be right since the quad have a V shape and would require two triangles 
	// to represent its norms correctly.)

	GetPainter()->Initialize();

	float c2x = 1.0 / (float) this->GetColumns();
	float r2y = 1.0 / (float) this->GetRows();
	float z = this->GetMin();

	Vector nX = Vector(-1, 0, 0);
	Vector pX = Vector(+1, 0, 0);

	Vector nY = Vector(0, -1, 0);
	Vector pY = Vector(0, +1, 0);

	Vector nZ = Vector(0, 0, -1);

	for (int row = 0; row < this->GetRows() - 1; row++) {
		for (int column = 0; column < this->GetColumns() - 1; column++) {
			float x = column * c2x;
			float y = row * r2y;

			float a = this->Get(row, column);
			float b = this->Get(row, column + 1);
			float c = this->Get(row + 1, column + 1);
			float d = this->Get(row + 1, column);

			// Top face (CW)
			float A[] = { x, y, a };
			float B[] = { x + c2x, y, b };
			float C[] = { x + c2x, y + r2y, c };
			float D[] = { x, y + r2y, d };
			GetPainter()->Color(
				A, B, C, D,
				&norms[row][column],
				&norms[row][column + 1],
				&norms[row + 1][column + 1],
				&norms[row + 1][column]
				);

			if (row == 0) {
				// Back face
				float A[] = { x, y, a };
				float B[] = { x, y, z };
				float C[] = { x + c2x, y, z };
				float D[] = { x + c2x, y, b };
				GetPainter()->Color(A, B, C, D, &nY);
			}
			else if (row == this->GetRows() - 2) {
				// Front face
				float A[] = { x, y + r2y, d };
				float B[] = { x + c2x, y + r2y, c };
				float C[] = { x + c2x, y + r2y, z };
				float D[] = { x, y + r2y, z };
				GetPainter()->Color(A, B, C, D, &pY);
			}

			if (column == 0) {
				// Right face
				float A[] = { x, y, a };
				float B[] = { x, y + r2y, d };
				float C[] = { x, y + r2y, z };
				float D[] = { x, y, z };
				GetPainter()->Color(A, B, C, D, &nX);
			}
			else if (column == this->GetColumns() - 2) {
				// Left face
				float A[] = { x + c2x, y, b };
				float B[] = { x + c2x, y, z };
				float C[] = { x + c2x, y + r2y, z };
				float D[] = { x + c2x, y + r2y, c };
				GetPainter()->Color(A, B, C, D, &pX);
			}
		}
	}

	// Bottom face
	float A[] = { 0, 0, z };
	float B[] = { 0, this->GetRows() * r2y - r2y, z };
	float C[] = { this->GetColumns() * c2x - c2x, this->GetRows() * r2y - r2y, z };
	float D[] = { this->GetColumns() * c2x - c2x, 0, z };
	GetPainter()->Color(A, B, C, D, &nZ);

	GetPainter()->Deinitialize();
}