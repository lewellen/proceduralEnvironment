#ifndef __WaterShader_H__
#define __WaterShader_H__

#include "Shader.h"
#include "FrameBuffer.h"

// Shader used for creating realistic looking water effects.
class WaterShader : public Shader {
private:
	GLint time, minSeaFloor, seaFloor, applyWave, refleTex, refraTex, bumpTex, depthTex;

public:
	// Initializes a new instance.
	WaterShader();
	
	// Releases memory associated with the instance.
	~WaterShader();

	// Sets the underlying time.
	void SetTime(float time);

	// Sets the underlying min sea floor (the deepest point of the sea floor).
	void SetMinSeaFloor(float minSeaFloor);

	// Sets the depth of the seafloor at the current vertex position.
	void SetSeaFloor(float seaFloor);

	// Sets the apply wave value. 1.0 for true, 0.0 for false.
	void SetApplyWave(float applyWave);
	
	// Sets the normal displacement texture handle.
	void SetBumpTexture(int i);

	// Sets the reflection texture handle.
	void SetReflectionTexture(int i);

	// Sets the refraction texture handle.
	void SetRefractionTexture(int i);

	// Sets the depth buffer from refraction render texture handle.
	void SetDepthTexture(int i);
};

#endif
