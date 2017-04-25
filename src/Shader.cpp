#include <cstdio>

#include "OpenGL.h"

#include "Shader.h"

#include "SchreuderUtil.h"

Shader::Shader(char* const vertexFilePath, char* const fragmentFilePath) {
	shaderHandle = CreateShaderProg(vertexFilePath, fragmentFilePath);
}

Shader::~Shader() {
	glDeleteProgram(shaderHandle);
	shaderHandle = -1;
}

void Shader::Begin() {
	glUseProgram(shaderHandle);
}

void Shader::End() {
	glUseProgram(GL_NONE);
}

GLint Shader::attribute(const char* name) {
	return glGetAttribLocation(shaderHandle, name);
}

void Shader::setAttributef(GLint location, float value) {
	if (location >= 0)
		glVertexAttrib1f(location, value);
}

void Shader::setUniformf(GLint location, float value) {
	if (location >= 0)
		glUniform1f(location, value);
}

void Shader::setUniformi(GLint location, int value) {
	if (location >= 0)
		glUniform1i(location, value);
}

GLint Shader::uniform(const char* name) {
	return glGetUniformLocation(shaderHandle, name);
}