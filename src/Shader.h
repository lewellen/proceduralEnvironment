#ifndef __Shader_H__
#define __Shader_H__

#include "OpenGL.h"

// Wraps the mechanics of using a GLSL shader in OpenGL.
class Shader {
private:
	int shaderHandle;

protected:
	// Retrieves the location of the specified named attribute.
	GLint attribute(const char* name);

	// Retrieves the location of the specified name uniform.
	GLint uniform(const char* name);

	// Assigns the given value to the specified attribute location.
	void setAttributef(GLint location, float value);

	// Assigns the given value to the specified uniform location.
	void setUniformf(GLint location, float value);

	// Assigns the given value to the specified uniform location.
	void setUniformi(GLint location, int value);

public:
	// Initializes a new instance given the vertex and fragment shader file 
	// locations.
	Shader(char* const vertexFilePath, char* const fragmentFilePath);
	
	// Releases memory associated with the instance.
	virtual ~Shader();

	// Inacts the underlying OpenGL mechanics to begin using the shader.
	virtual void Begin();

	// Inacts the underlying OpenGL mechanics to end use of the shader.
	virtual void End();
};

#endif
