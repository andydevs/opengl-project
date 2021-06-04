#pragma once
#include "GLObject.h"
#include <GL/glew.h>


class Shader : public GLObject
{
private:
	GLenum m_shaderType;
	const char* m_shaderFilename;
	const char* m_shaderSource;
public:
	Shader(GLenum shaderType, const char* shaderFilename);
	~Shader();
	void compile();
};

