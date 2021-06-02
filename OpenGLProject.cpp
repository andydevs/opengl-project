// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"

// Ooooh Data
#define NUM_VERTICES 4
#define NUM_POSITION_DIMENSIONS 2
const float geometry[NUM_VERTICES * NUM_POSITION_DIMENSIONS] = {
	-0.5f, -0.5f,
	-0.5f,  0.5f,
	 0.5f,  0.5f,
	 0.5f, -0.5f
};
#define NUM_COLOR_DIMENSIONS 3
const float color[NUM_VERTICES * NUM_COLOR_DIMENSIONS] = {
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0
};
#define NUM_TRIANGLES 2
#define VERT_PER_TRIANGLE 3
const unsigned indices[NUM_TRIANGLES * VERT_PER_TRIANGLE] = {
	0, 1, 2,
	0, 2, 3
};

// Resources
GLFWwindow* window;
Shader* vertexShader;
Shader* fragmentShader;
ShaderProgram* shaderProgram;
GLuint gPositionBuffer;
GLuint gColorBuffer;
GLuint gIndexBuffer;


void setupOpenGL()
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
	window = glfwCreateWindow(1280, 720, "OpenGL Test", nullptr, nullptr);
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
}

void useObject()
{
	glBindBuffer(GL_ARRAY_BUFFER, gPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void renderStep()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindBuffer(GL_ARRAY_BUFFER, gPositionBuffer);
	glVertexAttribPointer(0, NUM_POSITION_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer);
	glVertexAttribPointer(1, NUM_COLOR_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
	glDrawElements(GL_TRIANGLE_STRIP, NUM_TRIANGLES * VERT_PER_TRIANGLE, GL_UNSIGNED_INT, indices);
	glFlush();
}

int main() 
{
	// Setup opengl
	setupOpenGL();

	// Compile and link shader program
	vertexShader = new Shader(GL_VERTEX_SHADER, "shader.vert");
	vertexShader->compile();
	fragmentShader = new Shader(GL_FRAGMENT_SHADER, "shader.frag");
	fragmentShader->compile();
	shaderProgram = new ShaderProgram();
	shaderProgram->vertexShader(vertexShader);
	shaderProgram->fragmentShader(fragmentShader);
	shaderProgram->link();

	// Set up opengl buffers
	glGenBuffers(1, &gPositionBuffer);
	glGenBuffers(1, &gColorBuffer);

	// Set object and shader
	shaderProgram->bind();
	useObject();

	// Set up the whole loop thing
	while (!glfwWindowShouldClose(window))
	{
		// Render step
		renderStep();

		// Update glfw
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Teardown stuff
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
	glfwDestroyWindow(window);

	// Exit
	glfwTerminate();
	return EXIT_SUCCESS;
}