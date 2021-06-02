#pragma once
#include <GL/glew.h>


class Shader
{
private:
	GLenum m_shaderType;
	const char* m_shaderFilename;
	const char* m_shaderSource;
	GLint m_shaderHandle;
public:
	Shader(GLenum shaderType, const char* shaderFilename);
	~Shader();
	void compile();
	GLint handle();
};

