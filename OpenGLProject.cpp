// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.h"
#include "Debug.h"

// Ooooh Data
#define NUM_VERTICES 4
#define NUM_POSITION_DIMENSIONS 3
const float geometry[NUM_VERTICES * NUM_POSITION_DIMENSIONS] = {
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};
#define NUM_COLOR_DIMENSIONS 3
const float color[NUM_VERTICES * NUM_COLOR_DIMENSIONS] = {
	0.0, 0.0, 1.0,
	0.0, 1.0, 0.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 0.0
};
#define NUM_TRIANGLES 2
#define VERT_PER_TRIANGLE 3
const unsigned indices[NUM_TRIANGLES * VERT_PER_TRIANGLE] = {
	0, 1, 2,
	0, 2, 3
};

// Window
GLFWwindow* window;

// Shaders
Shader* vertexShader;
Shader* fragmentShader;
ShaderProgram* shaderProgram;

// Buffers
GLuint gPositionBuffer;
GLuint gColorBuffer;
GLuint uMatrixTransform;
GLuint uMatrixCamera;
GLuint uMatrixProjection;

// Transform and projection matrices
glm::mat4 transform;
glm::mat4 camera;
glm::mat4 projection;

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
	GL_SAFE_CALL(glGenBuffers(1, &gPositionBuffer));
	GL_SAFE_CALL(glGenBuffers(1, &gColorBuffer));
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, gPositionBuffer));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW));
	GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer));
	GL_SAFE_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW));

	// Set object and shader
	shaderProgram->bind();
	GL_SAFE_CALL(uMatrixTransform = glGetUniformLocation(shaderProgram->handle(), "transform"));
	GL_SAFE_CALL(uMatrixCamera = glGetUniformLocation(shaderProgram->handle(), "camera"));
	GL_SAFE_CALL(uMatrixProjection = glGetUniformLocation(shaderProgram->handle(), "projection"));

	// Set up the whole loop thing
	float time = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// Transform matrix
		transform  = glm::mat4(1.0f);
		transform  = glm::translate(transform, glm::vec3(sinf(time), 0.0, 0.0));
		transform  = glm::rotate(transform, time, glm::vec3(0.0, 0.0, 1.0));
		transform  = glm::scale(transform, glm::vec3(0.1f * cosf(5.0f*time) + 1.0f, 0.2f * cosf(2.0f*time + 0.2f) + 1.0f , 0.0f));
		camera     = glm::lookAt(glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
		projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.0f, 100.0f);

		// Render step
		GL_SAFE_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT));
		GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, gPositionBuffer));
		GL_SAFE_CALL(glVertexAttribPointer(0, NUM_POSITION_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0));
		GL_SAFE_CALL(glEnableVertexAttribArray(0));
		GL_SAFE_CALL(glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer));
		GL_SAFE_CALL(glVertexAttribPointer(1, NUM_COLOR_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0));
		GL_SAFE_CALL(glEnableVertexAttribArray(1));
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixTransform, 1, GL_FALSE, glm::value_ptr(transform)));
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixCamera, 1, GL_FALSE, glm::value_ptr(camera)));
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixProjection, 1, GL_FALSE, glm::value_ptr(projection)));
		GL_SAFE_CALL(glDrawElements(GL_TRIANGLE_STRIP, NUM_TRIANGLES * VERT_PER_TRIANGLE, GL_UNSIGNED_INT, indices));
		GL_SAFE_CALL(glFlush());

		// Update glfw
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Increment time
		time += 0.01;
	}

	// Teardown stuff
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
	GL_SAFE_CALL(glDeleteBuffers(1, &gPositionBuffer));
	GL_SAFE_CALL(glDeleteBuffers(1, &gColorBuffer));
	glfwDestroyWindow(window);

	// Exit
	glfwTerminate();
	return EXIT_SUCCESS;
}