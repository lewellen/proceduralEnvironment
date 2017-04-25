#ifndef __Water_H__
#define __Water_H__

#include "FrameBuffer.h"
#include "Renderable.h"
#include "Terrain.h"
#include "WaterShader.h"

// Procedurally generated water.
class Water : public Renderable {
private:
	float elevation;
	Terrain* terrain;
	
	float time, timeStep;

	WaterShader* waterShader;
	FrameBuffer* reflectionBuffer;
	FrameBuffer* refractionBuffer;

	int waterBumpTexture, plainWaterTexture;

	void setVertex(float x, float y, float z, float w, bool applyWave);

public:
	// Creates a new instance with the water line given by the elevation, to be 
	// placed on top of the specified Terrain.
	Water(float elevation, Terrain* Terrain);

	// Releases memory associated with the instance.
	~Water();

	// Inherited from Renderable. Updates the water's position.
	void DoIdle();

	// Inherited from Renderable. Draws the water.
	void Render();

	// Stores the framebuffers so that they can be used to render water effects.
	void SetBuffers(FrameBuffer* reflection, FrameBuffer* refraction);
};

#endif
