#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Debug.h"

/// <summary>
/// Construct Shader Program
/// </summary>
ShaderProgram::ShaderProgram():
	GLObject(),
	m_vertexShader(nullptr),
	m_fragmentShader(nullptr) {}

/// <summary>
/// Destruct Shader Program
/// </summary>
ShaderProgram::~ShaderProgram()
{
	// Delete handle if exist
	if (m_handle) {
		GL_SAFE_CALL(glDeleteProgram(m_handle))
	}
}

/// <summary>
/// Set vertex shader
/// </summary>
/// 
/// <param name="vs">Compiled vertex shader object</param>
void ShaderProgram::vertexShader(Shader* vs)
{
	m_vertexShader = vs;
}

/// <summary>
/// Set fragment shader
/// </summary>
/// 
/// <param name="fs">Compiled fragment shader object</param>
void ShaderProgram::fragmentShader(Shader* fs)
{
	m_fragmentShader = fs;
}

/// <summary>
/// Link whole shader
/// </summary>
void ShaderProgram::link()
{
	if (m_vertexShader && m_fragmentShader)
	{
		// Buffers n shoch
		GLint isGood;
		GLint maxlen;

		// Create program and attach shaders and link
		GL_SAFE_CALL(m_handle = glCreateProgram());
		GL_SAFE_CALL(glAttachShader(m_handle, m_vertexShader->handle()));
		GL_SAFE_CALL(glAttachShader(m_handle, m_fragmentShader->handle()));
		GL_SAFE_CALL(glLinkProgram(m_handle));

		// Handle errors
		GL_SAFE_CALL(glGetProgramiv(m_handle, GL_LINK_STATUS, &isGood));
		if (isGood == GL_FALSE)
		{
			std::cout << "Error linking shader program: " << std::endl;
			GL_SAFE_CALL(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &maxlen));
			std::vector<GLchar> infolog(maxlen);
			GL_SAFE_CALL(glGetProgramInfoLog(m_handle, maxlen, &maxlen, &infolog[0]));
			std::string infologstr(infolog.begin(), infolog.end());
			std::cout << infologstr << std::endl;
			__debugbreak();
		}
		else
		{
			std::cout << "Shader program linked successfully!" << std::endl;
		}
	}
}

/// <summary>
/// Bind shader to GL context
/// </summary>
void ShaderProgram::bind()
{
	GL_SAFE_CALL(glUseProgram(m_handle));
}

/// <summary>
/// Get uniform handle for given name
/// </summary>
/// 
/// <param name="name">Name of handle</param>
/// 
/// <returns>Return uniform handle</returns>
GLuint ShaderProgram::uniformHandle(const char* name)
{
	GLuint uniformHandle;
	GL_SAFE_CALL(uniformHandle = glGetUniformLocation(m_handle, name));
	return uniformHandle;
}

/// <summary>
/// Set matrix uniform for given name
/// </summary>
/// 
/// <param name="name">name of uniform</param>
/// <param name="matrix">matrix value</param>
void ShaderProgram::setUniformMatrix4fv(const char* name, glm::mat4 matrix)
{
	GLuint uniformHandle;
	GL_SAFE_CALL(uniformHandle = glGetUniformLocation(m_handle, name));
	GL_SAFE_CALL(glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, glm::value_ptr(matrix)));
}

/// <summary>
/// Set vector uniform for the given name
/// </summary>
/// 
/// <param name="name">name of uniform</param>
/// <param name="vector">vector value</param>
void ShaderProgram::setUniform3fv(const char* name, glm::vec3 vector)
{
	GLuint uniformHandle;
	GL_SAFE_CALL(uniformHandle = glGetUniformLocation(m_handle, name));
	GL_SAFE_CALL(glUniform3fv(uniformHandle, 1, glm::value_ptr(vector)));
}