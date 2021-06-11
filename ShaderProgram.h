#pragma once
#include <GL/GLEW.h>
#include <glm/glm.hpp>
#include "Shader.h"

/// <summary>
/// Full shader program
/// </summary>
class ShaderProgram : public GLObject
{
private:
	/// <summary>
	/// Vertex shader
	/// </summary>
	Shader* m_vertexShader;

	/// <summary>
	/// Fragment shader
	/// </summary>
	Shader* m_fragmentShader;
public:
	/// <summary>
	/// Construct Shader Program
	/// </summary>
	ShaderProgram();

	/// <summary>
	/// Destruct Shader Program
	/// </summary>
	~ShaderProgram();

	/// <summary>
	/// Set vertex shader
	/// </summary>
	/// 
	/// <param name="vs">Compiled vertex shader object</param>
	void vertexShader(Shader* vs);

	/// <summary>
	/// Set fragment shader
	/// </summary>
	/// 
	/// <param name="fs">Compiled fragment shader object</param>
	void fragmentShader(Shader* fs);

	/// <summary>
	/// Link whole shader
	/// </summary>
	void link();

	/// <summary>
	/// Bind shader to GL context
	/// </summary>
	void bind();

	/// <summary>
	/// Get uniform handle for given name
	/// </summary>
	/// 
	/// <param name="name">Name of handle</param>
	/// 
	/// <returns>Return uniform handle</returns>
	GLuint uniformHandle(const char* name);

	/// <summary>
	/// Set matrix uniform for given name
	/// </summary>
	/// 
	/// <param name="name">name of uniform</param>
	/// <param name="matrix">matrix value</param>
	void setUniformMatrix4fv(const char* name, glm::mat4 matrix);

	/// <summary>
	/// Set vector uniform for the given name
	/// </summary>
	/// 
	/// <param name="name">name of uniform</param>
	/// <param name="vector">vector value</param>
	void setUniform3fv(const char* name, glm::vec3 vector);
};