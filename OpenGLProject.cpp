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
#include "Debug.h"

// Ooooh Data
#define NUM_VERTICES 8
#define NUM_POSITION_DIMENSIONS 3
const float geometry[NUM_VERTICES * NUM_POSITION_DIMENSIONS] = {
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,

};
#define NUM_TEXCOORD_DIMENSIONS 2
const float texcoord[NUM_VERTICES * NUM_TEXCOORD_DIMENSIONS] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
};
#define NUM_NORMAL_DIMENSIONS 3
const float normal[NUM_VERTICES * NUM_NORMAL_DIMENSIONS] = {
	-0.58, -0.58, -0.58,
	-0.58,  0.58, -0.58,
	 0.58,  0.58, -0.58,
	 0.58, -0.58, -0.58,

	-0.58, -0.58,  0.58,
	-0.58,  0.58,  0.58,
	 0.58,  0.58,  0.58,
	 0.58, -0.58,  0.58
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
	0, 4, 5, 
	0, 5, 1,

	// Left
	3, 7, 6,
	3, 6, 2,

	// Top
	1, 5, 6,
	1, 6, 2,

	// Bottom
	0, 4, 7,
	0, 7, 3
};

// Window
GLFWwindow* window;

// Shaders
Shader* vertexShader;
Shader* fragmentShader;
ShaderProgram* shaderProgram;

// Texture
GLuint textureHandle;

// Mesh
Mesh* mesh;

// Uniforms
GLuint uMatrixTransform;
GLuint uMatrixCamera;
GLuint uMatrixProjection;
GLuint uVectorAmbientLight;
GLuint uVectorRimLight;
GLuint uVectorDirectionalLightColor;
GLuint uVectorDirectionalLightVector;

// Transform and projection matrices
glm::mat4 transform;
glm::mat4 camera;
glm::mat4 projection;
glm::vec3 ambientLight;
glm::vec3 rimLight;
glm::vec3 directionalLightColor;
glm::vec3 directionalLightVector;

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

	// Set swap speed
	glfwSwapInterval(1);
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

	// Get uniform locations
	uMatrixTransform = shaderProgram->uniformHandle("transform");
	uMatrixCamera = shaderProgram->uniformHandle("camera");
	uMatrixProjection = shaderProgram->uniformHandle("projection");
	uVectorRimLight = shaderProgram->uniformHandle("rimLight");
	uVectorAmbientLight = shaderProgram->uniformHandle("ambientLight");
	uVectorDirectionalLightColor = shaderProgram->uniformHandle("directionalLightColor");
	uVectorDirectionalLightVector = shaderProgram->uniformHandle("directionalLightVector");

	// Create mesh object
	mesh = new Mesh(NUM_VERTICES,
		NUM_POSITION_DIMENSIONS, geometry,
		NUM_TEXCOORD_DIMENSIONS, texcoord,
		NUM_NORMAL_DIMENSIONS, normal);

	// Load texture
	textureHandle = SOIL_load_OGL_texture("brick-texture.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS);
	if (textureHandle == 0) {
		std::cout << "ERROR Loading Texture: " << SOIL_last_result() << std::endl;
		__debugbreak();
	}

	// Enable vertex attrib arrays
	GL_SAFE_CALL(glEnableVertexAttribArray(0));
	GL_SAFE_CALL(glEnableVertexAttribArray(1));
	GL_SAFE_CALL(glEnableVertexAttribArray(2));

	// Set up the whole loop thing
	float time = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// Transform matrix
		transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, time, glm::vec3(0.0, 0.0, 1.0));
		transform = glm::rotate(transform, 0.7f * time, glm::vec3(0.0, 1.0, 0.0));
		transform = glm::rotate(transform, 0.3f * time, glm::vec3(1.0, 0.0, 0.0));
		camera = glm::lookAt(glm::vec3(0.0, 0.0, -6.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));
		projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 10.0f);

		// Lighting
		rimLight = glm::vec3(0.4f, 0.4f, 0.4f);
		ambientLight = glm::vec3(0.2f, 0.2f, 0.2f);
		directionalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		directionalLightVector = glm::normalize(glm::vec3(0.85, 0.8, 0.75));
		
		// Initialize render step
		GL_SAFE_CALL(glEnable(GL_DEPTH_TEST));
		GL_SAFE_CALL(glDepthFunc(GL_LESS));
		GL_SAFE_CALL(glClearColor(0.0, 0.0, 0.0, 1.0));
		GL_SAFE_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Set shader and mesh
		shaderProgram->bind();
		mesh->setToRender();

		// Set texture
		GL_SAFE_CALL(glActiveTexture(GL_TEXTURE0));
		GL_SAFE_CALL(glBindTexture(GL_TEXTURE_2D, textureHandle));

		// Set uniforms
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixTransform, 1, GL_FALSE, glm::value_ptr(transform)));
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixCamera, 1, GL_FALSE, glm::value_ptr(camera)));
		GL_SAFE_CALL(glUniformMatrix4fv(uMatrixProjection, 1, GL_FALSE, glm::value_ptr(projection)));
		GL_SAFE_CALL(glUniform3fv(uVectorRimLight, 1, glm::value_ptr(rimLight)));
		GL_SAFE_CALL(glUniform3fv(uVectorAmbientLight, 1, glm::value_ptr(ambientLight)));
		GL_SAFE_CALL(glUniform3fv(uVectorDirectionalLightColor, 1, glm::value_ptr(directionalLightColor)));
		GL_SAFE_CALL(glUniform3fv(uVectorDirectionalLightVector, 1, glm::value_ptr(directionalLightVector)));

		// Draw
		GL_SAFE_CALL(glDrawElements(GL_TRIANGLES, NUM_TRIANGLES * VERT_PER_TRIANGLE, GL_UNSIGNED_INT, indices));
		GL_SAFE_CALL(glFlush());

		// Update glfw
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Increment time
		time += 1.0f/60.0f;
	}

	// Teardown stuff
	delete mesh;
	delete shaderProgram;
	delete fragmentShader;
	delete vertexShader;
	GL_SAFE_CALL(glDeleteTextures(1, &textureHandle));
	glfwDestroyWindow(window);

	// Exit
	glfwTerminate();
	return EXIT_SUCCESS;
}