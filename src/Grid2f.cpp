#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdexcept>

#include "Grid2f.h"

Grid2f::Grid2f(int rows, int columns) {
	this->array = (float*) malloc(rows * columns * sizeof(float));
	this->rows = rows;
	this->columns = columns;

	Zero();
}

Grid2f::~Grid2f() {
	free(this->array);
}

int Grid2f::Rows() {
	return this->rows;
}

int Grid2f::Columns() {
	return this->columns;
}

float Grid2f::Get(int row, int column) {
	if (row < 0 || row >= this->rows)
		throw std::invalid_argument("row must be non-negative and less than this->rows.");

	if (column < 0 || column >= this->columns)
		throw std::invalid_argument("column must be non-negative and less than this->columns.");

	return this->array[row * rows + column];
}

void Grid2f::Set(int row, int column, float f) {
	if (row < 0 || row >= this->rows)
		throw std::invalid_argument("row must be non-negative and less than this->rows.");

	if (column < 0 || column >= this->columns)
		throw std::invalid_argument("column must be non-negative and less than this->columns.");

	this->array[row * rows + column] = f;
}

void Grid2f::Zero() {
	memset(this->array, 0, rows * columns * sizeof(float));
}