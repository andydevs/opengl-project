// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Ooooh Data
#define NUM_DIMENSIONS 2
#define NUM_VERTICES 3
const float geometry[NUM_DIMENSIONS * NUM_VERTICES] = {
	-0.5f, -0.5f,
	0.0, 0.5,
	0.5, -0.5
};

// Buffers n soch
GLuint gVertexBuffer;

int main() {
	// Initialize GLFW
	// Exit if didn't work
	if (!glfwInit()) {
		std::cout << "GLFW failed to initialize: the hell would I know what happened" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// Create window
	// Again, exit if didn't work
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Test", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cout << "GLEW failed to initialize: " << glewGetErrorString(status) << std::endl;
		return -1;
	}

	// Set up opengl buffers
	glGenBuffers(1, &gVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

	// Set up the whole loop thing
	while (!glfwWindowShouldClose(window)) {
		// Render step
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
		glVertexAttribPointer(0, NUM_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
		glFlush();

		// Update glfw
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}