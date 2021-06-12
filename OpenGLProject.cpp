// OpenGLProject.cpp : Contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstring>
#include "setupOpenGL.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "ArrayBuffer.h"
#include "Texture.h"
#include "Debug.h"

/// <summary>
/// Main function
/// </summary>
/// 
/// <returns>Status code upon completion</returns>
int main(int argc, const char** argv) 
{
	// Ensure we get 6 args
	if (argc < 7) {
		std::cout << "ERROR: program requires 6 arguments!" << std::endl;
		std::cout << "> OpenGLProject [width] [height] [vertex shader] [fragment shader] [texture] [mesh]" << std::endl;
		__debugbreak();
		return EXIT_FAILURE;
	}

	// Get arguments
	const unsigned width = atoi(argv[1]);
	const unsigned height = atoi(argv[2]);
	const char* vertexShaderFilename = argv[3];
	const char* fragmentShaderFilename = argv[4];
	const char* textureFilename = argv[5];
	const char* meshFilename = argv[6];
	const char* windowName = "OpenGL Project";

	// Setup opengl window
	GLFWwindow* window = setupOpenGL(width, height, windowName);

	// Compile and link shader program
	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, vertexShaderFilename);
	vertexShader->compile();
	Shader* fragmentShader = new Shader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	fragmentShader->compile();
	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->vertexShader(vertexShader);
	shaderProgram->fragmentShader(fragmentShader);
	shaderProgram->link();

	// Load texture
	Texture* texture = new Texture(textureFilename);
	std::cout << "Loaded texture" << std::endl;

	// Read mesh object
	Mesh* mesh = Mesh::readObj(meshFilename);
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