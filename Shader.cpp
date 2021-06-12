// Shader.cpp: Implementation for Shader object
//
#include "Shader.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Debug.h"

/// <summary>
/// Construct shader from file
/// </summary>
/// <param name="shaderType">Type of shader being created</param>
/// <param name="shaderFilename">Filename to read code from</param>
Shader::Shader(GLenum shaderType, const char* shaderFilename)
	: GLObject(),
	m_shaderType(shaderType),
	m_shaderFilename(_strdup(shaderFilename))
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

/// <summary>
/// Delete shader on GPU if exists
/// </summary>
Shader::~Shader()
{
	if (m_handle) {
		GL_SAFE_CALL(glDeleteShader(m_handle));
	}
	delete m_shaderSource;
	delete m_shaderFilename;
}

/// <summary>
/// Compile shader
/// </summary>
void Shader::compile()
{
	// Parameters n soch
	GLint isGood;
	GLint maxlen;

	// Compile shader
	GL_SAFE_CALL(m_handle = glCreateShader(m_shaderType));
	GL_SAFE_CALL(glShaderSource(m_handle, 1, &m_shaderSource, 0));
	GL_SAFE_CALL(glCompileShader(m_handle));

	// Check errors
	GL_SAFE_CALL(glGetShaderiv(m_handle, GL_COMPILE_STATUS, &isGood));
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling shader " << m_shaderFilename << std::endl;
		GL_SAFE_CALL(glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &maxlen));
		std::vector<GLchar> infolog(maxlen);
		GL_SAFE_CALL(glGetShaderInfoLog(m_handle, maxlen, &maxlen, &infolog[0]));
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		__debugbreak();
	}
	else
	{
		std::cout << "Successfuly compiled shader " << m_shaderFilename << std::endl;
	}
}