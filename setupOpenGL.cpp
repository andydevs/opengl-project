// setupOpenGL.cpp: Implementation for setupOpenGL function
//
#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "setupOpenGL.h"

/// <summary>
/// Sets up a window and an OpenGL context
/// </summary>
/// 
/// <param name="width">Width of the window</param>
/// <param name="height">Height of the window</param>
/// <param name="title">Window title (that appears on the top left)</param>
/// 
/// <returns>GLFW window instance</returns>
GLFWwindow* setupOpenGL(unsigned width, unsigned height, const char* title)
{
	// Initialize GLFW
	// Exit if didn't work
	if (!glfwInit())
	{
		std::cout
			<< "GLFW failed to initialize: "
			<< "the hell would I know what happened"
			<< std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	std::cout
		<< "Initialized GLFW"
		<< std::endl;

	// Create window
	// Again, exit if didn't work
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		std::cout
			<< "GLFW failed to create a window"
			<< std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	std::cout
		<< "Create window"
		<< std::endl;

	// Initialize opengl through GLEW
	// Exit if don't work
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout
			<< "GLEW failed to initialize: "
			<< glewGetErrorString(status)
			<< std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout
		<< "OpenGL Version "
		<< glGetString(GL_VERSION)
		<< std::endl;

	// Set swap speed
	glfwSwapInterval(1);

	// Return final window
	return window;
}