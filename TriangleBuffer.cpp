// TriangleBuffer.cpp: Implementation for TriangleBuffer object
//
#include "TriangleBuffer.h"
#include "Debug.h"
#include <GL/glew.h>

/// <summary>
/// Construct TriangleBuffer
/// </summary>
/// 
/// <param name="numTriangles">number of triangles</param>
/// <param name="indices">buffer of indices</param>
TriangleBuffer::TriangleBuffer(unsigned numTriangles, const unsigned* indices):
	m_numTriangles(numTriangles),
	m_indices(new unsigned[numTriangles*VERT_PER_TRIANGLE]) 
{
	for (size_t i = 0; i < numTriangles*VERT_PER_TRIANGLE; ++i) {
		m_indices[i] = indices[i];
	}
}

/// <summary>
/// Destruct TriangleBuffer
/// </summary>
TriangleBuffer::~TriangleBuffer()
{
	delete m_indices;
}

/// <summary>
/// Number of total indices
/// </summary>
/// 
/// <returns>number of total indices</returns>
unsigned TriangleBuffer::numIndices()
{
	return m_numTriangles * VERT_PER_TRIANGLE;
}

/// <summary>
/// Draw triangle buffer to context
/// </summary>
void TriangleBuffer::draw()
{
	GL_SAFE_CALL(glDrawElements(GL_TRIANGLES, this->numIndices(), GL_UNSIGNED_INT, m_indices));
}