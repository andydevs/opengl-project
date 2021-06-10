#include "Mesh.h"
#include "Debug.h"

Mesh::Mesh(
	unsigned numVertices, 
	unsigned positionDim, const float* position, 
	unsigned texcoordDim, const float* texcoord,
	unsigned normalDim, const float* normal,
	unsigned numTriangles, const unsigned* indices) :
	m_aPositionBuffer(new ArrayBuffer(numVertices, positionDim, position)),
	m_aTexcoordBuffer(new ArrayBuffer(numVertices, texcoordDim, texcoord)),
	m_aNormalBuffer(new ArrayBuffer(numVertices, normalDim, normal)),
	m_numTriangles(numTriangles),
	m_indices(new unsigned[numTriangles*VERT_PER_TRIANGLE])
{
	for (size_t i = 0; i < numTriangles*VERT_PER_TRIANGLE; i++) {
		m_indices[i] = indices[i];
	}
}

Mesh::~Mesh()
{
	delete m_aPositionBuffer;
	delete m_aTexcoordBuffer;
	delete m_aNormalBuffer;
	delete m_indices;
}

void Mesh::draw()
{
	m_aPositionBuffer->setToAttribute(0);
	m_aTexcoordBuffer->setToAttribute(1);
	m_aNormalBuffer->setToAttribute(2);
	GL_SAFE_CALL(glDrawElements(GL_TRIANGLES, 
		m_numTriangles * VERT_PER_TRIANGLE, 
		GL_UNSIGNED_INT, m_indices));
}

void Mesh::setToRender()
{
	m_aPositionBuffer->setToAttribute(0);
	m_aTexcoordBuffer->setToAttribute(1);
	m_aNormalBuffer->setToAttribute(2);
}