#include "Mesh.h"

Mesh::Mesh(
	unsigned numVertices, 
	unsigned positionDim, const float* position, 
	unsigned texcoordDim, const float* texcoord, 
	unsigned colorDim, const float* color, 
	unsigned normalDim, const float* normal) :
	m_aPositionBuffer(new ArrayBuffer(numVertices, positionDim, position)),
	m_aColorBuffer(new ArrayBuffer(numVertices, colorDim, color)),
	m_aTexcoordBuffer(new ArrayBuffer(numVertices, texcoordDim, texcoord)),
	m_aNormalBuffer(new ArrayBuffer(numVertices, normalDim, normal)) 
{}

Mesh::~Mesh()
{
	delete m_aPositionBuffer;
	delete m_aColorBuffer;
	delete m_aNormalBuffer;
}

void Mesh::setToRender()
{
	m_aPositionBuffer->setToAttribute(0);
	m_aTexcoordBuffer->setToAttribute(1);
	m_aColorBuffer->setToAttribute(2);
	m_aNormalBuffer->setToAttribute(3);
}