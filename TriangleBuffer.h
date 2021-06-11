#pragma once
#include "GLObject.h"

#define VERT_PER_TRIANGLE 3

class TriangleBuffer
{
private:
	unsigned m_numTriangles;
	unsigned* m_indices;
public:
	TriangleBuffer(unsigned numTriangles, const unsigned* indices);
	~TriangleBuffer();
	unsigned numIndices();
	void draw();
};

