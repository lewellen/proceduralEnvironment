#ifndef __Placeable_H__
#define __Placeable_H__

// An object that can be placed on a grid.
class Placeable {
private:
	int row, column;
	float scale;

public:
	// Initializes a new instance placed at the origin.
	Placeable();

	// Gets the column where the item is placed.
	int GetColumn();

	// Gets the number columns this takes up.
	virtual int GetColumnFootPrint() = 0;

	// Gets the row where the item is placed.
	int GetRow();

	// Gets the number of rows this takes up.
	virtual int GetRowFootPrint() = 0;

	// Gets the scale at which the object will be drawn.
	float GetScale();

	// Sets both the row and column where the item is placed.
	void Set(int row, int column);

	// Sets the scale that the object will be drawn.
	void SetScale(float value);
};

#endif