#include "Shader.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Debug.h"

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
	if (m_shaderHandle) {
		GL_SAFE_CALL(glDeleteShader(m_shaderHandle));
	}
	delete m_shaderSource;
	delete m_shaderFilename;
}

void Shader::compile()
{
	// Parameters n soch
	GLint isGood;
	GLint maxlen;

	// Compile shader
	GL_SAFE_CALL(m_shaderHandle = glCreateShader(m_shaderType));
	GL_SAFE_CALL(glShaderSource(m_shaderHandle, 1, &m_shaderSource, 0));
	GL_SAFE_CALL(glCompileShader(m_shaderHandle));

	// Check errors
	GL_SAFE_CALL(glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &isGood));
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling shader " << m_shaderFilename << std::endl;
		GL_SAFE_CALL(glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &maxlen));
		std::vector<GLchar> infolog(maxlen);
		GL_SAFE_CALL(glGetShaderInfoLog(m_shaderHandle, maxlen, &maxlen, &infolog[0]));
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		__debugbreak();
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