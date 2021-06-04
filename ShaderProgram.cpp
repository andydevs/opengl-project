#include "ShaderProgram.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Debug.h"

ShaderProgram::ShaderProgram(): 
	m_vertexShader(nullptr),
	m_fragmentShader(nullptr),
	m_shaderProgram(0) {}

ShaderProgram::~ShaderProgram()
{
	// Delete handle if exist
	if (m_shaderProgram) {
		GL_SAFE_CALL(glDeleteProgram(m_shaderProgram))
	}
}

void ShaderProgram::vertexShader(Shader* vs)
{
	m_vertexShader = vs;
}

void ShaderProgram::fragmentShader(Shader* fs)
{
	m_fragmentShader = fs;
}

void ShaderProgram::link()
{
	if (m_vertexShader && m_fragmentShader)
	{
		// Buffers n shoch
		GLint isGood;
		GLint maxlen;

		// Create program and attach shaders and link
		GL_SAFE_CALL(m_shaderProgram = glCreateProgram());
		GL_SAFE_CALL(glAttachShader(m_shaderProgram, m_vertexShader->handle()));
		GL_SAFE_CALL(glAttachShader(m_shaderProgram, m_fragmentShader->handle()));
		GL_SAFE_CALL(glLinkProgram(m_shaderProgram));

		// Handle errors
		GL_SAFE_CALL(glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &isGood));
		if (isGood == GL_FALSE)
		{
			std::cout << "Error linking shader program: " << std::endl;
			GL_SAFE_CALL(glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &maxlen));
			std::vector<GLchar> infolog(maxlen);
			GL_SAFE_CALL(glGetProgramInfoLog(m_shaderProgram, maxlen, &maxlen, &infolog[0]));
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

void ShaderProgram::bind()
{
	GL_SAFE_CALL(glUseProgram(m_shaderProgram));
}