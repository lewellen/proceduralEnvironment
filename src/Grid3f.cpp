#include <cstdio>	 // NULL
#include <cstdlib>	 // free, malloc
#include <cstring>	 // memset
#include <stdexcept> // std::invalid_argument
//#include <unordered_set>

#include "Grid3f.h"

Grid3f::Grid3f(int columns, int rows, int planes) {
	if (columns <= 0 || columns > 255)
		throw new std::invalid_argument("Columns must be greater than zero and less than equal to 255.");

	if (rows <= 0 || columns > 255)
		throw new std::invalid_argument("Rows must be greater than zero and less than equal to 255.");

	if (planes <= 0 || columns > 255)
		throw new std::invalid_argument("Planes must be greater than zero and less than equal to 255.");

	this->columns = columns;
	this->rows = rows;
	this->planes = planes;

	data = (float*) malloc( columns * rows * planes * sizeof(float) );
}

Grid3f::~Grid3f() {
	free(data);
	data = NULL;
}

int Grid3f::Columns() {
	return columns;
}

int Grid3f::Rows() {
	return rows;
}

int Grid3f::Planes() {
	return planes;
}

//void Grid3f::BeginIterate() {
//	setValuesCursor = 0;
//}
//
//bool Grid3f::Next(int* column, int* row, int* plane) {
//	if (setValuesCursor == setValueseNext)
//		return false;
//
//	while (setValuesCursor < setValueseNext && setValues[setValuesCursor] < 0)
//		setValuesCursor++;
//
//	int key = setValues[setValuesCursor];
//
//	*column = keyToColumn(key);
//	*row = keyToRow(key);
//	*plane = keyToPlane(key);
//	
//	setValuesCursor++;
//
//	return true;
//}

float Grid3f::Get(int column, int row, int plane) {
	checkAccess(column, row, plane);

	return data[column + rows * (row + planes * plane)];
}

void Grid3f::Set(int column, int row, int plane, float x) {
	checkAccess(column, row, plane);

	int index = indexOf(column, row, plane);
	data[index] = x;

	if (x < 0)
		x = -x;

	if (x > 0.0001) {
	
	}
}

void Grid3f::Zero() {
	memset(data, 0, columns * rows * planes * sizeof(float));
}

inline void Grid3f::checkAccess(int column, int row, int plane) {
	if (column < 0 || column >= columns)
		throw new std::invalid_argument("Column must be between 0 and [columns].");

	if (row < 0 || row >= rows)
		throw new std::invalid_argument("Row must be between 0 and [rows].");

	if (plane < 0 || plane >= planes)
		throw new std::invalid_argument("Plane must be between 0 and [planes].");
}

inline int Grid3f::indexOf(int column, int row, int plane) {
	return column + rows * (row + planes * plane);
}

//inline int Grid3f::keyOf(int column, int row, int plane) {
//	return (column & 255) << 16 | (row & 255) << 8 | (plane & 255);
//}
//
//inline int Grid3f::keyToColumn(int key) {
//	return (key >> 16) & 255;
//}
//
//inline int Grid3f::keyToRow(int key) {
//	return (key >> 8) & 255;
//}
//
//inline int Grid3f::keyToPlane(int key) {
//	return key & 255;
//}