#ifndef __House_H__
#define __House_H__

#include "Placeable.h"
#include "Renderable.h"
#include "SmokePlume.h"

// Unit house that can be visualized.
class House : public Placeable, public Renderable {
private:
	int rotation;
	float scale;

	int logTexture, logEndTexture, roofTexture, roofSideTexture, 
		doorTexture, windowTexture, chimneyTexture;

	SmokePlume* plume;
	bool smokeEnabled;

	void chimneyTop();

	void log(float length, float radius);
	void wall(int logs);
	void wedge();

	void texturedCube(float x, float y, float z, int topTexture, int sidesTexture);
	void texturedQuad(float width, float height, int texture);

public:
	// Initializes a new instance with the given rotation and scale.
	House(int rotation, float scale);

	// Inherited from Renderable.
	void DoIdle();

	// Gets the rotation of the house.
	int GetRotation();

	// Inherited rom Placeable. Gets the number of rows this takes up.
	int GetRowFootPrint();

	// Inherited rom Placeable. Gets the number columns this takes up.
	int GetColumnFootPrint();

	// Gets the scale of the house.
	float GetScale();

	// Inherited from Renderable. Draws the house.
	void Render();

	// Turns the smoke plume on or off.
	void ToggleSmoke();
};

#endif
