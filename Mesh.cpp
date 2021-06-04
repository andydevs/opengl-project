#include "Mesh.h"

Mesh::Mesh(unsigned numVertices, unsigned positionDim, const float* position, unsigned colorDim, const float* color, unsigned normalDim, const float* normal) :
	m_aPositionBuffer(new ArrayBuffer(numVertices, positionDim, position)),
	m_aColorBuffer(new ArrayBuffer(numVertices, colorDim, color)),
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
	m_aColorBuffer->setToAttribute(1);
	m_aNormalBuffer->setToAttribute(2);
}