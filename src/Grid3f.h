#ifndef __Grid3f_H__
#define __Grid3f_H__

//#include<unordered_set>

// Represents a three dimensional grid.
class Grid3f {
private:
	int columns;
	int rows;
	int planes;

	float* data;
	inline void checkAccess(int column, int row, int plane);
	inline int indexOf(int column, int row, int plane);

	// std::unordered_set<int> setValues;
	//inline int keyOf(int column, int row, int plane);
	//inline int keyToColumn(int column);
	//inline int keyToRow(int key);
	//inline int keyToPlane(int plane);
	
public:
	// Initializes a new instance with the specified dimensions.
	Grid3f(int columns, int rows, int planes);

	// Releases memory associated with the instance.
	~Grid3f();

	// Gets the number of columns.
	int Columns();

	// Gets the number of rows.
	int Rows();

	// Gets the number planes.
	int Planes();

	//void BeginIterate();
	//bool Next(int* column, int* row, int* plane);

	// Gets the value at the specified coordinates.
	float Get(int column, int row, int plane);

	// Sets the value of the specified coordinates.
	void Set(int column, int row, int plane, float x);

	// Sets all entries in the grid to zero.
	void Zero();
};

#endif
