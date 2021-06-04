#include "GLObject.h"

GLObject::GLObject() : m_handle(0) {}

GLObject::~GLObject() {}

GLuint GLObject::handle()
{
	return m_handle;
}