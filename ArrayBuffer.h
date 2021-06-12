// ArrayBuffer.h: Interface for ArrayBuffer object
//
#pragma once
#include "GLObject.h"

/// <summary>
/// Contains array of data. Stored in GPU
/// </summary>
class ArrayBuffer: public GLObject
{
private:
    /// <summary>
    /// Height/length of array data
    /// </summary>
    unsigned m_height;

    /// <summary>
    /// Width/stride of array data
    /// </summary>
    unsigned m_width;
public:
    /// <summary>
    /// Construct ArrayBuffer
    /// </summary>
    /// <param name="height">Height/length of array data</param>
    /// <param name="width">Width/stride of array data</param>
    /// <param name="data">Data points to store in buffer</param>
    ArrayBuffer(unsigned height, unsigned width, const float* data);

    /// <summary>
    /// Destruct ArrayBuffer
    /// </summary>
    ~ArrayBuffer();

    /// <summary>
    /// Get height/length of ArrayBuffer
    /// </summary>
    /// 
    /// <returns>height/length of ArrayBuffer</returns>
    unsigned height();

    /// <summary>
    /// Get width/stride of ArrayBuffer
    /// </summary>
    /// 
    /// <returns>width/stride of ArrayBuffer</returns>
    unsigned width();

    /// <summary>
    /// Bind array buffer to GL context
    /// </summary>
    void bind();

    /// <summary>
    /// Set to vertex attrib array number
    /// </summary>
    /// 
    /// <param name="index">vertex attrib array number</param>
    void setToAttribute(GLint index);
};