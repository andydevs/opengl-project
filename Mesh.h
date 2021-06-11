#pragma once
#include "ArrayBuffer.h"
#include "TriangleBuffer.h"

class Mesh
{
private:
	ArrayBuffer* m_aPositionBuffer;
	ArrayBuffer* m_aTexcoordBuffer;
	ArrayBuffer* m_aNormalBuffer;
	TriangleBuffer* m_triangleBuffer;
	unsigned m_numTriangles;
	unsigned* m_indices;
public:
	static Mesh* readObj(const char* filename);
	Mesh(
		unsigned numVertices,
		unsigned positionDim,
		const float* position,
		unsigned texcoordDim,
		const float* texcoord,
		unsigned normalDim,
		const float* normal,
		unsigned numTriangles,
		const unsigned* indices);
	~Mesh();
	void draw();
};