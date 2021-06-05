#pragma once
#include "ArrayBuffer.h"

class Mesh
{
private:
	ArrayBuffer* m_aPositionBuffer;
	ArrayBuffer* m_aColorBuffer;
	ArrayBuffer* m_aTexcoordBuffer;
	ArrayBuffer* m_aNormalBuffer;
public:
	Mesh(
		unsigned numVertices,
		unsigned positionDim,
		const float* position,
		unsigned texcoordDim,
		const float* texcoord,
		unsigned colorDim,
		const float* color,
		unsigned normalDim,
		const float* normal);
	~Mesh();
	void setToRender();
};