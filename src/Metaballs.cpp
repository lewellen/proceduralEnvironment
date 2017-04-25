#include <cstdio>
#include <cmath>

#include "Metaballs.h"

#include "Grid3f.h"
#include "Vector.h"

void Metaballs::ToField(Vector* points, int numPoints, Grid3f* data) {
	data->Zero();

	int R = 3;

	float unit = 1.0 / fmin(fmin(data->Columns(), data->Rows()), data->Planes());
	float ballRadius = 4 * unit;
	float RSquared = ballRadius * ballRadius;

	// O( numPoints )
	for (int i = 0; i < numPoints; i++) {
		// Map the point to its nearest cell based on (x, y, z) values.
		int column = round(points[i].x * data->Columns());
		int row = round(points[i].y * data->Rows());
		int plane = round(points[i].z * data->Planes());

		if (
			(column < 0 || column >= data->Columns()) ||
			(row < 0 || row >= data->Rows()) ||
			(plane < 0 || plane >= data->Planes())
			)
			continue;

		// Iterate over the neighborhood of the point and add in the point's 
		// potential to the field.
		for (int u = -R; u <= R; u++){
			for (int v = -R; v <= R; v++) {
				for (int w = -R; w <= R; w++) {
					if (
						(column + u < 0 || column + u >= data->Columns()) ||
						(row + v < 0 || row + v >= data->Rows()) ||
						(plane + w < 0 || plane + w >= data->Planes())
						)
						continue;

					float x = (column + u) * unit;
					float y = (row + v) * unit;
					float z = (plane + w) * unit;

					// Distance from point to cell
					float rSquared =
						(x - points[i].x) * (x - points[i].x)
						+ (y - points[i].y) * (y - points[i].y)
						+ (z - points[i].z) * (z - points[i].z);

					// http://people.cs.clemson.edu/~dhouse/courses/405/notes/blobbies/blobbies.html
					if (rSquared <= RSquared) {
						float rho2 = rSquared / RSquared;
						float rho4 = rho2 * rho2;
						float rho6 = rho4 * rho2;

						float fxyz = data->Get(column + u, row + v, plane + w);
						fxyz += 1.0 - 3.0 * rho2 + 3 * rho4 - rho6;
						data->Set(column + u, row + v, plane + w, fxyz);
					}
				}
			}
		}
	}
}