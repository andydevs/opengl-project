#include "Shader.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(GLenum shaderType, const char* shaderFilename)
	: m_shaderType(shaderType),
	m_shaderFilename(_strdup(shaderFilename)),
	m_shaderHandle(0)
{	
	// Read file contents
	const int MAX_LEN = 2048;
	char str[MAX_LEN];
	std::ifstream sourceFile(m_shaderFilename);
	std::stringstream sourceBuff;
	while (sourceFile.good()) {
		sourceFile.getline(str, MAX_LEN);
		sourceBuff << str << std::endl;
	}
	sourceFile.close();
	m_shaderSource = _strdup(sourceBuff.str().c_str());
}

Shader::~Shader()
{
	if (m_shaderHandle) glDeleteShader(m_shaderHandle);
	delete m_shaderSource;
	delete m_shaderFilename;
}

void Shader::compile()
{
	// Parameters n soch
	GLint isGood;
	GLint maxlen;

	// Compile shader
	m_shaderHandle = glCreateShader(m_shaderType);
	glShaderSource(m_shaderHandle, 1, &m_shaderSource, 0);
	glCompileShader(m_shaderHandle);

	// Check errors
	glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling shader " << m_shaderFilename << std::endl;
		glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetShaderInfoLog(m_shaderHandle, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		exit(EXIT_FAILURE); // TODO: DONT DO THIS HORRIBLE THING
	}
	else
	{
		std::cout << "Successfuly compiled shader " << m_shaderFilename << std::endl;
	}
}

GLint Shader::handle()
{
	return m_shaderHandle;
}