#include "Texture.h"
#include "Debug.h"
#include <GL/glew.h>
#include <soil.h>
#include <cstring>
#include <iostream>

/// <summary>
/// Construct texture object
/// </summary>
/// 
/// <param name="filename">Name of file to read from</param>
Texture::Texture(const char* filename)
	: GLObject(), 
	m_filename(_strdup(filename)) 
{
	m_handle = SOIL_load_OGL_texture(filename, 
		SOIL_LOAD_RGB, 
		SOIL_CREATE_NEW_ID, 
		SOIL_FLAG_TEXTURE_REPEATS);
	if (m_handle == 0) {
		std::cout << "ERROR Loading Texture: " << SOIL_last_result() << std::endl;
		__debugbreak();
	}
}

/// <summary>
/// Destruct texture object
/// </summary>
Texture::~Texture()
{
	GL_SAFE_CALL(glDeleteTextures(1, &m_handle));
}

/// <summary>
/// Set texture to render
/// </summary>
void Texture::setToRender()
{
	GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
	GL_SAFE_CALL(glBindTexture(GL_TEXTURE_2D, m_handle));
}