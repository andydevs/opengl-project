#include "Mesh.h"
#include "Debug.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>

// Maximum line width when reading .obj
const unsigned LINE_WIDTH = 2048;

// .obj line matchers
const std::regex vertex("v\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)(?:\\s+([\\+\\-]?\\d+\\.\\d+))?\\s*");
const std::regex texcoord("vt\\s+([\\+\\-]?\\d+\\.\\d+)(?:\\s+([\\+\\-]?\\d+\\.\\d+))?(?:\\s+([\\+\\-]?\\d+\\.\\d+))?\\s*");
const std::regex normal("vn\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s*");
const std::regex face("f\\s+(\\d+)/\\1/\\1\\s+(\\d+)/\\2/\\2\\s+(\\d+)/\\3/\\3\\s*");

/// <summary>
/// Apply data from the matched .obj expression for the given type 
/// to the given buffer. This will also update the stride of the 
/// buffer to the maximum dimension of the received .obj expression 
/// of this type
/// </summary>
/// 
/// <param name="result">Match result generated containing data</param>
/// <param name="buffer">Buffer containing data for this type</param>
/// <param name="stride">Holds the final stride of the data</param>
void applyToBuffer(std::cmatch& result, std::vector<float>& buffer, unsigned* stride)
{
	// Compute length of match result (in float values)
	unsigned length = 0;
	for (size_t i = 1; i < result.size(); ++i)
	{
		if (result[i].length() > 0)
		{
			buffer.push_back(std::stof(result[i]));
			++length;
		}
	}

	// Update width
	if (length > *stride)
	{
		*stride = length;
	}
}

/// <summary>
/// Apply index data from the matchecd .obj face experession to the given buffer.
/// </summary>
/// 
/// <param name="result">Match result generated containing data</param>
/// <param name="indexBuffer">Buffer containing index data</param>
void applyToIndexBuffer(std::cmatch& result, std::vector<unsigned>& indexBuffer)
{
	for (size_t i = 1; i < result.size(); ++i)
	{
		indexBuffer.push_back((unsigned)std::stoul(result[i]) - 1);
	}
}

/// <summary>
/// Load .obj file as Mesh
/// </summary>
/// 
/// <param name="filename">Name of file to parse</param>
/// 
/// <returns>Parsed mesh object (or nullptr)</returns>
Mesh* Mesh::readObj(const char* filename) 
{
	// Widths of each set
	unsigned vertexStride = 0;
	unsigned texcoordStride = 0;
	unsigned normalStride = 0;

	// Buffers
	std::vector<float> vertexPositions;
	std::vector<float> vertexTexcoords;
	std::vector<float> vertexNormals;
	std::vector<unsigned> indices;

	// File stream
	std::cout << "Reading " << filename << "..." << std::endl;
	std::ifstream objin(filename);
	try 
	{
		// Go through file lines
		std::cmatch result;
		char line[LINE_WIDTH];
		while (objin.good()) 
		{
			// Read line
			objin.getline(line, LINE_WIDTH);

			// Match vertex
			if (std::regex_match(line, result, vertex)) 
			{
				applyToBuffer(result, vertexPositions, &vertexStride);
			}
			// Match texcoord
			else if (std::regex_match(line, result, texcoord))
			{
				applyToBuffer(result, vertexTexcoords, &texcoordStride);
			}
			// Match normal
			else if (std::regex_match(line, result, normal))
			{
				applyToBuffer(result, vertexNormals, &normalStride);
			}
			// Match indices
			else if (std::regex_match(line, result, face))
			{
				applyToIndexBuffer(result, indices);
			}
		}
	}
	// Close file handle
	catch (std::exception& e)
	{
		objin.close();
		return nullptr;
	}
	objin.close();

	// Get resulting number of vertices, texcoords, normals, and triangles
	unsigned vertices = vertexPositions.size() / vertexStride;
	unsigned texcoords = vertexTexcoords.size() / texcoordStride;
	unsigned normals = vertexNormals.size() / normalStride;
	unsigned triangles = indices.size() / VERT_PER_TRIANGLE;

	// Check if there are equal numbers of positions, tex coords, and normals
	if (vertices == texcoords && texcoords == normals && normals == vertices)
	{
		// Return mesh if passed
		std::cout << "Valid .obj file!" << std::endl;
		return new Mesh(vertices,
			vertexStride, vertexPositions.data(),
			texcoordStride, vertexTexcoords.data(),
			normalStride, vertexNormals.data(),
			triangles, indices.data());
	}
	else
	{
		// Print error if not
		std::cout << "Inputs are not equal!" << std::endl;
		return nullptr;
	}
}

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
Mesh::Mesh(
	unsigned numVertices, 
	unsigned positionDim, const float* position, 
	unsigned texcoordDim, const float* texcoord,
	unsigned normalDim, const float* normal,
	unsigned numTriangles, const unsigned* indices) :
	m_aPositionBuffer(new ArrayBuffer(numVertices, positionDim, position)),
	m_aTexcoordBuffer(new ArrayBuffer(numVertices, texcoordDim, texcoord)),
	m_aNormalBuffer(new ArrayBuffer(numVertices, normalDim, normal)),
	m_triangleBuffer(new TriangleBuffer(numTriangles, indices)) {}

/// <summary>
/// Destruct mesh
/// </summary>
Mesh::~Mesh()
{
	delete m_aPositionBuffer;
	delete m_aTexcoordBuffer;
	delete m_aNormalBuffer;
	delete m_triangleBuffer;
}

/// <summary>
/// Draw mesh
/// </summary>
void Mesh::draw()
{
	m_aPositionBuffer->setToAttribute(0);
	m_aTexcoordBuffer->setToAttribute(1);
	m_aNormalBuffer->setToAttribute(2);
	m_triangleBuffer->draw();
}