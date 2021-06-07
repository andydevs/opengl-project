#pragma once
#include "GLObject.h"

class Texture : public GLObject
{
private:
	const char* m_filename;
public:
	Texture(const char* filename);
	virtual ~Texture();
	void setToRender();
};

