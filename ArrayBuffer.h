#pragma once
#include "GLObject.h"

class ArrayBuffer :
    public GLObject
{
private:
    unsigned m_height;
    unsigned m_width;
public:
    ArrayBuffer(unsigned height, unsigned width, const float* data);
    ~ArrayBuffer();
    unsigned height();
    unsigned width();
    void bind();
    void setToAttribute(GLint index);
};