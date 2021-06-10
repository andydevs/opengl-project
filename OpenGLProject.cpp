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

// Ooooh Data
#define NUM_VERTICES 24
#define NUM_POSITION_DIMENSIONS 3
const float geometry[NUM_VERTICES * NUM_POSITION_DIMENSIONS] = {
	// Front
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	
	// Back
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,

	// Left
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	// Right
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,

	// Top
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,

	// Bottom
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f
};
#define NUM_TEXCOORD_DIMENSIONS 2
const float texcoord[NUM_VERTICES * NUM_TEXCOORD_DIMENSIONS] = {
	// Front
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	// Back
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	// Left
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	// Right
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	// Top
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	// Bottom
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};
#define NUM_NORMAL_DIMENSIONS 3
const float normal[NUM_VERTICES * NUM_NORMAL_DIMENSIONS] = {
	// Front
	 0.0,  0.0, -1.0,
	 0.0,  0.0, -1.0,
	 0.0,  0.0, -1.0,
	 0.0,  0.0, -1.0,

	// Back
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,
	 0.0,  0.0,  1.0,

	// Left
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,

	// Right
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,
	 1.0,  0.0,  0.0,

	// Top
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,
	 0.0,  1.0,  0.0,

	// Bottom
	 0.0, -1.0,  0.0,
	 0.0, -1.0,  0.0,
	 0.0, -1.0,  0.0,
	 0.0, -1.0,  0.0,
};
#define NUM_TRIANGLES 12
#define VERT_PER_TRIANGLE 3
const unsigned indices[NUM_TRIANGLES * VERT_PER_TRIANGLE] = {
	// Front
	0, 1, 2, 
	0, 2, 3,
	
	// Back
	4, 5, 6, 
	4, 6, 7,
	
	// Right
	8, 9, 10,
	8, 10, 11,

	// Left
	12, 13, 14,
	12, 14, 15,

	// Top
	16, 17, 18,
	16, 18, 19,

	// Bottom
	20, 21, 22,
	20, 22, 23
};

// Window
GLFWwindow* window;

// Resources
Shader* vertexShader;
Shader* fragmentShader;
ShaderProgram* shaderProgram;
Texture* texture;
Mesh* mesh;

// Transform and projection matrices
glm::mat4 transform;
glm::mat4 camera;
glm::mat4 projection;
glm::vec3 ambientLight;
glm::vec3 rimLight;
glm::vec3 directionalLightColor;
glm::vec3 directionalLightVector;

void setupOpenGL(unsigned width, unsigned height, const char* title)
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
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
}

int main() 
{
	// Height and width
	int width = 1280;
	int height = 720;

	// Setup opengl
	setupOpenGL(width, height, "OpenGL Project");

	// Compile and link shader program
	vertexShader = new Shader(GL_VERTEX_SHADER, "shader.vert");
	vertexShader->compile();
	fragmentShader = new Shader(GL_FRAGMENT_SHADER, "shader.frag");
	fragmentShader->compile();
	shaderProgram = new ShaderProgram();
	shaderProgram->vertexShader(vertexShader);
	shaderProgram->fragmentShader(fragmentShader);
	shaderProgram->link();

	// Load texture
	texture = new Texture("brick-texture.jpg");
	std::cout << "Loaded texture" << std::endl;

	// Read mesh object
	mesh = Mesh::readObj("cube.obj");
	if (!mesh)
	{
		std::cout << "Something went wrong reading mesh" << std::endl;
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

	// Set up the whole loop thing
	float time = 0.0f;
	camera = glm::lookAt(glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(45.0f, (float)width / height, 0.1f, 10.0f);
	while (!glfwWindowShouldClose(window))
	{
		// Transform matrix
		transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, time, glm::vec3(0.0, 0.0, 1.0));
		transform = glm::rotate(transform, 0.7f * time, glm::vec3(0.0, 1.0, 0.0));
		transform = glm::rotate(transform, 0.3f * time, glm::vec3(1.0, 0.0, 0.0));

		// Lighting
		rimLight = glm::vec3(0.3f, 0.3f, 0.3f);
		ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
		directionalLightColor = glm::vec3(0.3f, 0.3f, 0.3f);
		directionalLightVector = glm::normalize(glm::vec3(0.85, 0.8, 0.75));
		
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
		time += 1.0f/60.0f;
	}

	// Teardown stuff
	delete mesh;
	delete texture;
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
	glfwDestroyWindow(window);

	// Exit
	glfwTerminate();
	return EXIT_SUCCESS;
}