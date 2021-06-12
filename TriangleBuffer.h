#pragma once
#include "GLObject.h"

// Vertices per triangles
#define VERT_PER_TRIANGLE 3

/// <summary>
/// Buffer of indices organized for drawing triangles
/// </summary>
class TriangleBuffer
{
private:
	/// <summary>
	/// Number of triangles in buffer
	/// </summary>
	unsigned m_numTriangles;

	/// <summary>
	/// Buffer of indices of triangles
	/// </summary>
	unsigned* m_indices;
public:
	/// <summary>
	/// Construct TriangleBuffer
	/// </summary>
	/// 
	/// <param name="numTriangles">number of triangles</param>
	/// <param name="indices">buffer of indices</param>
	TriangleBuffer(unsigned numTriangles, const unsigned* indices);

	/// <summary>
	/// Destruct TriangleBuffer
	/// </summary>
	~TriangleBuffer();

	/// <summary>
	/// Number of total indices
	/// </summary>
	/// 
	/// <returns>number of total indices</returns>
	unsigned numIndices();

	/// <summary>
	/// Draw triangle buffer to context
	/// </summary>
	void draw();
};

