// ArrayBuffer.cpp: Implementation for ArrayBuffer object
//
#include "ArrayBuffer.h"
#include "Debug.h"

/// <summary>
/// Construct ArrayBuffer
/// </summary>
/// <param name="height">Height/length of array data</param>
/// <param name="width">Width/stride of array data</param>
/// <param name="data">Data points to store in buffer</param>
ArrayBuffer::ArrayBuffer(unsigned height, unsigned width, const float* data)
	: GLObject(),
	m_height(height),
	m_width(width)
{
	GL_SAFE_CALL(glGenBuffers(1, &m_handle));
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, height*width*sizeof(float), data, GL_STATIC_DRAW));
}

/// <summary>
/// Destruct ArrayBuffer
/// </summary>
ArrayBuffer::~ArrayBuffer()
{
	if (m_handle) {
		GL_SAFE_CALL(glDeleteBuffers(1, &m_handle));
	}
}

/// <summary>
/// Get height/length of ArrayBuffer
/// </summary>
/// 
/// <returns>height/length of ArrayBuffer</returns>
unsigned ArrayBuffer::height()
{
	return m_height;
}

/// <summary>
/// Get width/stride of ArrayBuffer
/// </summary>
/// 
/// <returns>width/stride of ArrayBuffer</returns>
unsigned ArrayBuffer::width()
{
	return m_width;
}

/// <summary>
/// Bind array buffer to GL context
/// </summary>
void ArrayBuffer::bind()
{
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_handle));
}

/// <summary>
/// Set to vertex attrib array number
/// </summary>
/// 
/// <param name="index">vertex attrib array number</param>
void ArrayBuffer::setToAttribute(GLint index)
{
	this->bind();
	GL_SAFE_CALL(glVertexAttribPointer(index, this->width(), GL_FLOAT, GL_FALSE, 0, 0));
}