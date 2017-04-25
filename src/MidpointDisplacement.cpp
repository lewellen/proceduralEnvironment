#include <cstdio>
#include <cmath>
#include <stdexcept>

#include "Random.h"
#include "MidpointDisplacement.h"

MidpointDisplacement::MidpointDisplacement() {

}

MidpointDisplacement::~MidpointDisplacement() {

}

Grid2f* MidpointDisplacement::Populate(int edgeSize, float a, float b, float c, float d) {
	// Calculate the row and column sizes.
	if (edgeSize < 1 || edgeSize > 15)
		throw std::invalid_argument("edgeSize must be 1 and 15.");

	edgeSize = (1 << edgeSize) + 1;

	Grid2f* A = new Grid2f(edgeSize, edgeSize);

	// Seed with default values.
	A->Set(0, 0, a);
	A->Set(0, A->Columns() - 1, b);
	A->Set(A->Rows() - 1, 0, c);
	A->Set(A->Rows() - 1, A->Columns() - 1, d);

	// Generate the terrain
	populate(A, 0, 0, A->Rows(), A->Columns());

	// smooth out the rough parts
	smooth(&A);
	smooth(&A);

	return A;
}

Vector** MidpointDisplacement::Norms(Grid2f* A) {
	Vector** norms = allocNorms(A->Rows(), A->Columns());

	// Calculate all the normals
	float c2x = 1.0 / (float)A->Columns();
	float r2y = 1.0 / (float)A->Rows();

	for (int row = 1; row < A->Rows() - 1; row++) {
		for (int column = 1; column < A->Columns() - 1; column++) {
			float a = A->Get(row, column);

			float b = A->Get(row, column + 1);
			float c = A->Get(row + 1, column);
			float d = A->Get(row, column - 1);
			float e = A->Get(row - 1, column);

			// Calculate the central difference about (row, column) in the x 
			// direction, and then in the y direction; use the cross product 
			// of the two as the norm.
			Vector x = Vector((2.0 * c2x), 0, (b - d));
			Vector y = Vector(0, (2.0 * r2y), (c - e));
			Vector* z = x.Cross(&y)->Normalize();

			norms[row][column] = *z;

			delete z;
		}
	}

	// fill in the border
	for (int s = 0; s < A->Rows(); s++) {
		norms[s][0] = Vector(norms[s][1]);
		norms[0][s] = Vector(norms[1][s]);
		norms[s][A->Columns() - 1] = Vector(norms[s][A->Columns() - 2]);
		norms[A->Rows() - 1][s] = Vector(norms[A->Rows() - 2][s]);
	}

	return norms;
}

Vector** MidpointDisplacement::allocNorms(int rows, int columns) {
	Vector** A = new Vector*[rows];
	for (int row = 0; row < rows; row++) {
		A[row] = new Vector[columns];
		for (int column = 0; column < columns; column++)
			A[row][column] = Vector(0, 0, 0);
	}

	return A;
}

void MidpointDisplacement::populate(Grid2f* A, int row, int column, int rows, int columns) {
	float a = A->Get(row, column);
	float b = A->Get(row, column + (columns - 1));
	float c = A->Get(row + (rows - 1), column);
	float d = A->Get(row + (rows - 1), column + (columns - 1));

	float min = fmin(fmin(a, b), fmin(c, d));
	float max = fmax(fmax(a, b), fmax(c, d));

	float ab = 0.5 * (a + b) + nextFloat(min, max);
	float bd = 0.5 * (b + d) + nextFloat(min, max);
	float dc = 0.5 * (d + c) + nextFloat(min, max);
	float ca = 0.5 * (a + c) + nextFloat(min, max);

	float center = 0.5 * (ab + dc) + nextFloat(min, max);

	//this->min = fmin(fmin(fmin(ab, bd), fmin(dc, ca)), fmin(this->min, center));
	//this->max = fmax(fmax(fmax(ab, bd), fmax(dc, ca)), fmax(this->min, center));

	A->Set(row, column + (columns / 2), ab);
	A->Set(row + (rows / 2), column + (columns - 1), bd);
	A->Set(row + (rows - 1), column + (columns / 2), dc);
	A->Set(row + (rows / 2), column, ca);
	A->Set(row + (rows / 2), column + (columns / 2), center);

	if (rows == 3 || columns == 3)
		return;

	int half = (rows / 2) + 1;

	populate(A, row, column, half, half);
	populate(A, row, column + (columns / 2), half, half);
	populate(A, row + (rows / 2), column, half, half);
	populate(A, row + (rows / 2), column + (columns / 2), half, half);
}

void MidpointDisplacement::smooth(Grid2f** aPtr) {
	// 2015-06-14 GEL Was a 3x3 mean filter; decided to change to 3x3 Gaussian 
	// filter. Should import a FFT to do this more efficently.

	float kernel[3][3] = {
			{ 1, 2, 1 },
			{ 2, 4, 2 },
			{ 1, 2, 1 }
	};

	int window = 1;

	Grid2f* A = *aPtr;

	Grid2f* store = new Grid2f(A->Rows(), A->Columns());
	for (int row = 0; row < A->Rows(); row++) {
		for (int column = 0; column < A->Columns(); column++) {
			float weight = 0;
			for (int u = -window; u <= window; u++) {
				for (int v = -window; v <= window; v++) {
					if (column + v >= 0 && column + v < A->Columns() && row + u >= 0 && row + u < A->Rows()) {
						float f = store->Get(row, column);
						f += A->Get(row + u, column + v) * kernel[u + 1][v + 1];

						store->Set(row, column, f);

						weight += kernel[u + window][v + window];
					}
				}
			}

			store->Set(row, column, store->Get(row, column) / weight);
		}
	}

	delete A;
	*aPtr = store;
}

float MidpointDisplacement::nextFloat(float min, float max) {
	return 2 * (0.5 - Random::Get()->NextFloat())*(max - min)*.3;
}