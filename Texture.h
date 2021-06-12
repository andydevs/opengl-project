// Texture.h: Interface for Texture object
// 
#pragma once
#include "GLObject.h"

/// <summary>
/// A texture loaded into OpenGL
/// </summary>
class Texture : public GLObject
{
private:
	/// <summary>
	/// Original filename of texture
	/// </summary>
	const char* m_filename;
public:
	/// <summary>
	/// Construct texture object
	/// </summary>
	/// 
	/// <param name="filename">Name of file to read from</param>
	Texture(const char* filename);

	/// <summary>
	/// Destruct texture object
	/// </summary>
	~Texture();

	/// <summary>
	/// Set texture to render
	/// </summary>
	void setToRender();
};

