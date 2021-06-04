#pragma once
#include <GL/GLEW.h>
#include "Shader.h"

class ShaderProgram
{
private:
	Shader* m_vertexShader;
	Shader* m_fragmentShader;
	GLuint m_shaderProgram;
public:
	ShaderProgram();
	~ShaderProgram();
	void vertexShader(Shader* vs);
	void fragmentShader(Shader* fs);
	void link();
	void bind();
	GLuint handle();
};