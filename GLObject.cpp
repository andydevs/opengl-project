// GLObject.cpp: Implementation for GLObject object
//
#include "GLObject.h"

/// <summary>
/// Construct GLObject (initialize handle to 0)
/// </summary>
GLObject::GLObject() : m_handle(0) {}

/// <summary>
/// Destruct Object (implemented in subclass)
/// </summary>
GLObject::~GLObject() {}

/// <summary>
/// Return object's OpenGL handle
/// </summary>
/// 
/// <returns>OpenGL handle</returns>
GLuint GLObject::handle()
{
	return m_handle;
}