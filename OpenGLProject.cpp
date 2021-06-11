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
#include <soil.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "ArrayBuffer.h"
#include "Texture.h"
#include "Debug.h"

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

/// <summary>
/// Main function
/// </summary>
/// 
/// <returns>Status code upon completion</returns>
int main() 
{
	// Setup opengl
	const unsigned width = 1280;
	const unsigned height = 720;
	GLFWwindow* window = setupOpenGL(width, height, "OpenGL Project");

	// Compile and link shader program
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "shader.vert");
	vertexShader->compile();
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, "shader.frag");
	fragmentShader->compile();
	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->vertexShader(vertexShader);
	shaderProgram->fragmentShader(fragmentShader);
	shaderProgram->link();

	// Load texture
	Texture* texture = new Texture("brick-texture.jpg");
	std::cout << "Loaded texture" << std::endl;

	// Read mesh object
	Mesh* mesh = Mesh::readObj("cube.obj");
	if (!mesh)
	{
		std::cout << "Something went wrong reading mesh" << std::endl;
		delete texture;
		delete shaderProgram;
		delete fragmentShader;
		delete vertexShader;
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	std::cout << "Mesh loaded successfully" << std::endl;

	// Enable vertex attrib arrays
	GL_SAFE_CALL(glEnableVertexAttribArray(0));
	GL_SAFE_CALL(glEnableVertexAttribArray(1));
	GL_SAFE_CALL(glEnableVertexAttribArray(2));

	// Transform
	glm::mat4 transform;

	// Camera
	const glm::mat4 camera = glm::lookAt(glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
	const glm::mat4 projection = glm::perspective(45.0f, (float)width / height, 0.1f, 10.0f);

	// Lighting
	const glm::vec3 rimLight = glm::vec3(0.3f, 0.3f, 0.3f);
	const glm::vec3 ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
	const glm::vec3 directionalLightColor = glm::vec3(0.3f, 0.3f, 0.3f);
	const glm::vec3 directionalLightVector = glm::normalize(glm::vec3(0.85, 0.8, 0.75));

	// Time and time step
	const float dtime = 1.0f / 60.0f;
	float time = 0.0f;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Transform matrix
		transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, time, glm::vec3(0.0, 0.0, 1.0));
		transform = glm::rotate(transform, 0.7f * time, glm::vec3(0.0, 1.0, 0.0));
		transform = glm::rotate(transform, 0.3f * time, glm::vec3(1.0, 0.0, 0.0));
		
		// Initialize render step
		GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glDepthFunc(GL_LESS));
		GL_SAFE_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Set resources (shader, uniforms, and textures)
		shaderProgram->bind();
		texture->setToRender();
		shaderProgram->setUniformMatrix4fv("transform", transform);
		shaderProgram->setUniformMatrix4fv("camera", camera);
		shaderProgram->setUniformMatrix4fv("projection", projection);
		shaderProgram->setUniform3fv("rimLight", rimLight);
		shaderProgram->setUniform3fv("ambientLight", ambientLight);
		shaderProgram->setUniform3fv("directionalLightColor", directionalLightColor);
		shaderProgram->setUniform3fv("directionalLightVector", directionalLightVector);

		// Draw mesh
		mesh->draw();

		// Update window
		GL_SAFE_CALL(glFlush());
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Increment time
		time += dtime;
	}

	// Teardown stuff and exit
	delete mesh;
	delete texture;
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}