#ifndef __ConeLight_H__
#define __ConeLight_H__

#include "OpenGL.h"

#include "Actor.h"
#include "IFrame.h"
#include "Lights.h"
#include "Rotation.h"
#include "Vector.h"

// Represents a directional spot light.
class ConeLight : public Actor {
private:
	GLenum light;
	GLfloat spotCutoff, spotExponent;
	lightDefinition definition;

public:
	// Initializes a new instance. Applies definition, spotCutoff, and 
	// spotExponent to the GL_LIGHTx given by light. Light position and 
	// direction determined by frame.
	ConeLight(IFrame* frame, GLenum light, lightDefinition* definition, float spotCutoff, float spotExponent);
	
	// Releases any memory associated with the instance.
	virtual ~ConeLight();

	// Inherited from Actor. Renders the light (both visual cone and sets up 
	// OpenGL lighting for the light).
	void Render();
};

#endif