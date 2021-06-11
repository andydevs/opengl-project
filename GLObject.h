#pragma once
#include <GL/glew.h>

/// <summary>
/// ABSTRACT
/// An OpenGL object stored on the GPU
/// subclass is responsible for implementing
/// create and bind logic for handle
/// </summary>
class GLObject
{
protected:
	/// <summary>
	/// Object's OpenGL handle
	/// </summary>
	GLuint m_handle;
public:
	/// <summary>
	/// Construct GLObject (initialize handle to 0)
	/// </summary>
	GLObject();

	/// <summary>
	/// Destruct Object (implemented in subclass)
	/// </summary>
	virtual ~GLObject();

	/// <summary>
	/// Return object's OpenGL handle
	/// </summary>
	/// 
	/// <returns>OpenGL handle</returns>
	GLuint handle();
};

