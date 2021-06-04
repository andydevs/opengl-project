#include "ArrayBuffer.h"
#include "Debug.h"

ArrayBuffer::ArrayBuffer(unsigned height, unsigned width, const float* data)
	: GLObject(),
	m_height(height),
	m_width(width)
{
	GL_SAFE_CALL(glGenBuffers(1, &m_handle));
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, height*width*sizeof(float), data, GL_STATIC_DRAW));
}

ArrayBuffer::~ArrayBuffer()
{
	if (m_handle) {
		GL_SAFE_CALL(glDeleteBuffers(1, &m_handle));
	}
}

unsigned ArrayBuffer::height()
{
	return m_height;
}

unsigned ArrayBuffer::width()
{
	return m_width;
}

void ArrayBuffer::bind()
{
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
}

void ArrayBuffer::setToAttribute(GLint index)
{
	this->bind();
	GL_SAFE_CALL(glVertexAttribPointer(index, this->width(), GL_FLOAT, GL_FALSE, 0, 0));
}