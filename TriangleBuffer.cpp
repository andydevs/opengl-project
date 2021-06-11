#include "TriangleBuffer.h"
#include "Debug.h"
#include <GL/glew.h>

TriangleBuffer::TriangleBuffer(unsigned numTriangles, const unsigned* indices):
	m_numTriangles(numTriangles),
	m_indices(new unsigned[numTriangles*VERT_PER_TRIANGLE]) 
{
	for (size_t i = 0; i < numTriangles*VERT_PER_TRIANGLE; ++i) {
		m_indices[i] = indices[i];
	}
}

TriangleBuffer::~TriangleBuffer()
{
	delete m_indices;
}

unsigned TriangleBuffer::numIndices()
{
	return m_numTriangles * VERT_PER_TRIANGLE;
}

void TriangleBuffer::draw()
{
	GL_SAFE_CALL(glDrawElements(GL_TRIANGLES, this->numIndices(), GL_UNSIGNED_INT, m_indices));
}