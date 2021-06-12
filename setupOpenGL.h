// setupOpenGL.h: Interface for setupOpenGL function
//
#pragma once
#include <GLFW/glfw3.h>

/// <summary>
/// Sets up a window and an OpenGL context
/// </summary>
/// 
/// <param name="width">Width of the window</param>
/// <param name="height">Height of the window</param>
/// <param name="title">Window title (that appears on the top left)</param>
/// 
/// <returns>GLFW window instance</returns>
GLFWwindow* setupOpenGL(unsigned width, unsigned height, const char* title);
