#include "Mesh.h"
#include "Debug.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>

#define LINE_WIDTH 2048

Mesh* Mesh::readObj(const char* filename) 
{
	// Widths of each set
	unsigned vertexWidth = 0;
	unsigned texcoordWidth = 0;
	unsigned normalWidth = 0;

	// Buffers
	std::vector<float> vertexPositions;
	std::vector<float> vertexTexcoords;
	std::vector<float> vertexNormals;
	std::vector<unsigned> indices;

	// Regexes
	std::regex vertex;
	std::regex texcoord;
	std::regex normal;
	std::regex face;
	try
	{
		vertex   = "v\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)(?:\\s+([\\+\\-]?\\d+\\.\\d+))?\\s*";
		texcoord = "vt\\s+([\\+\\-]?\\d+\\.\\d+)(?:\\s+([\\+\\-]?\\d+\\.\\d+))?(?:\\s+([\\+\\-]?\\d+\\.\\d+))?\\s*";
		normal   = "vn\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s+([\\+\\-]?\\d+\\.\\d+)\\s*";
		face     = "f\\s+(\\d+)/\\1/\\1\\s+(\\d+)/\\2/\\2\\s+(\\d+)/\\3/\\3\\s*";
	}
	catch (std::regex_error& e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}

	// File stream
	std::ifstream objin(filename);
	try 
	{
		// Go through file lines
		unsigned length;
		std::cmatch result;
		char line[LINE_WIDTH];
		while (objin.good()) 
		{
			// Read line
			objin.getline(line, LINE_WIDTH);

			// Match vertex
			if (std::regex_match(line, result, vertex)) 
			{
				// Count length and add vertex value
				length = 0;
				for (size_t i = 1; i < result.size(); i++)
				{
					if (result[i].length()) 
					{
						vertexPositions.push_back(std::stof(result[i]));
						++length;
					}
				}

				// Set vertex width if length is greater
				if (length > vertexWidth)
				{
					vertexWidth = length;
				}
			}
			// Match texcoord
			else if (std::regex_match(line, result, texcoord))
			{
				// Count length and add vertex value
				length = 0;
				for (size_t i = 1; i < result.size(); i++)
				{
					if (result[i].length())
					{
						vertexTexcoords.push_back(std::stof(result[i]));
						++length;
					}
				}

				// Set vertex width if length is greater
				if (length > texcoordWidth)
				{
					texcoordWidth = length;
				}
			}
			// Match normal
			else if (std::regex_match(line, result, normal))
			{
				// Count length and add vertex value
				length = 0;
				for (size_t i = 1; i < result.size(); i++)
				{
					if (result[i].length())
					{
						vertexNormals.push_back(std::stof(result[i]));
						++length;
					}
				}

				// Set vertex width if length is greater
				if (length > normalWidth)
				{
					normalWidth = length;
				}
			}
			// Match indices
			else if (std::regex_match(line, result, face))
			{
				// Count length and add vertex value
				for (size_t i = 1; i < result.size(); i++)
				{
					indices.push_back((unsigned)std::stoul(result[i]) - 1);
				}
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

	// Get results
	unsigned vertices = vertexPositions.size() / vertexWidth;
	unsigned texcoords = vertexTexcoords.size() / texcoordWidth;
	unsigned normals = vertexNormals.size() / normalWidth;
	unsigned triangles = indices.size() / VERT_PER_TRIANGLE;

	// Check
	if (vertices == texcoords && texcoords == normals && normals == vertices)
	{
		std::cout << "Valid .obj file!" << std::endl;
		return new Mesh(vertices,
			vertexWidth, vertexPositions.data(),
			texcoordWidth, vertexTexcoords.data(),
			normalWidth, vertexNormals.data(),
			triangles, indices.data());
	}
	else
	{
		std::cout << "Inputs are not equal!" << std::endl;
		return nullptr;
	}
}

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

Mesh::~Mesh()
{
	delete m_aPositionBuffer;
	delete m_aTexcoordBuffer;
	delete m_aNormalBuffer;
	delete m_triangleBuffer;
}

void Mesh::draw()
{
	m_aPositionBuffer->setToAttribute(0);
	m_aTexcoordBuffer->setToAttribute(1);
	m_aNormalBuffer->setToAttribute(2);
	m_triangleBuffer->draw();
}