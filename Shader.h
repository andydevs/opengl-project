// Shader.h: Interface for Shader object
//
#pragma once
#include "GLObject.h"
#include <GL/glew.h>

/// <summary>
/// A loaded OpenGL shader read from a file.
/// </summary>
class Shader : public GLObject
{
private:
	/// <summary>
	/// OpenGL constant
	/// The type of the shader (vertex, fragment, etc)
	/// </summary>
	GLenum m_shaderType;

	/// <summary>
	/// Original filename of shader
	/// </summary>
	const char* m_shaderFilename;

	/// <summary>
	/// Original source code of shader
	/// </summary>
	const char* m_shaderSource;
public:
	/// <summary>
	/// Construct shader from file
	/// </summary>
	/// <param name="shaderType">Type of shader being created</param>
	/// <param name="shaderFilename">Filename to read code from</param>
	Shader(GLenum shaderType, const char* shaderFilename);

	/// <summary>
	/// Delete shader on GPU if exists
	/// </summary>
	~Shader();

	/// <summary>
	/// Compile shader
	/// </summary>
	void compile();
};

