#ifndef __Grid2f_H__
#define __Grid2f_H__

// Represents a row major two dimensional array.
class Grid2f {
private:
	int rows, columns;
	float* array;

public:
	// Initializes a new instance with the given number of rows and columns.
	Grid2f(int rows, int columns);

	// Releases memory associated with the array.
	~Grid2f();

	// Underlying number of rows in the array.
	int Rows();

	// Underlying number of columns in the array.
	int Columns();

	// Retrieves the entry at [row, column].
	float Get(int row, int column);

	// Sets the entry at [row, column] = f.
	void Set(int row, int column, float f);

	// Zeros every single entry in the array to zero.
	void Zero();
};

#endif