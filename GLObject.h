#pragma once
#include <GL/glew.h>

class GLObject
{
protected:
	GLuint m_handle;
public:
	GLObject();
	virtual ~GLObject();
	GLuint handle();
};

