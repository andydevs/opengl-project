// Mesh.h: Interface for Mesh object
//
#pragma once
#include "ArrayBuffer.h"
#include "TriangleBuffer.h"

/// <summary>
/// Contains information for an OpenGL 
/// geometry. Contains buffers of vertex 
/// positions, texcoords, and normals, as 
/// well as buffer of 3D faces
/// </summary>
class Mesh
{
private:
	/// <summary>
	/// Position buffer
	/// </summary>
	ArrayBuffer* m_aPositionBuffer;

	/// <summary>
	/// Texcoord buffer
	/// </summary>
	ArrayBuffer* m_aTexcoordBuffer;

	/// <summary>
	/// Normal buffer
	/// </summary>
	ArrayBuffer* m_aNormalBuffer;

	/// <summary>
	/// Index buffer for triangle faces
	/// </summary>
	TriangleBuffer* m_triangleBuffer;
public:
	/// <summary>
	/// Load .obj file as Mesh
	/// </summary>
	/// 
	/// <param name="filename">Name of file to parse</param>
	/// 
	/// <returns>Parsed mesh object (or nullptr)</returns>
	static Mesh* readObj(const char* filename);

	/// <summary>
	/// Construct Mesh
	/// </summary>
	/// 
	/// <param name="numVertices">Number of mesh vertices</param>
	/// <param name="positionDim">Dimension of vertex positions</param>
	/// <param name="position">Vertex position buffer</param>
	/// <param name="texcoordDim">Dimension of vertex texture coordinates</param>
	/// <param name="texcoord">Vertex texcoord buffer</param>
	/// <param name="normalDim">Dimension of vertex normals</param>
	/// <param name="normal">Vertex normal buffer</param>
	/// <param name="numTriangles">Number of mesh triangles</param>
	/// <param name="indices">Mesh index buffer</param>
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

	/// <summary>
	/// Destruct mesh
	/// </summary>
	~Mesh();

	/// <summary>
	/// Draw mesh
	/// </summary>
	void draw();
};