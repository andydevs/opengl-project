#pragma once
#include <GL/GLEW.h>
#include "Shader.h"

class ShaderProgram : public GLObject
{
private:
	Shader* m_vertexShader;
	Shader* m_fragmentShader;
public:
	ShaderProgram();
	~ShaderProgram();
	void vertexShader(Shader* vs);
	void fragmentShader(Shader* fs);
	void link();
	void bind();
	GLuint uniformHandle(const char* name);
};