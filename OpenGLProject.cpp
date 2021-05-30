// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Ooooh Data
#define NUM_DIMENSIONS 2
#define NUM_VERTICES 3
const float geometry[NUM_VERTICES * NUM_DIMENSIONS] = {
	-0.5f, -0.5f,
	0.0, 0.5,
	0.5, -0.5
};

// Resources
GLFWwindow* window;
GLuint gVertexBuffer;
GLuint shaderVertex;
GLuint shaderFragment;
GLuint shaderProgram;

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
	window = glfwCreateWindow(
		640, 480, 
		"OpenGL Test", 
		nullptr, nullptr);
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

void compileShader() 
{
	// Other things
	GLint isGood;
	GLint maxlen;
#define MAX_LEN 2048
	char str[MAX_LEN];
	const GLchar* vertSource;
	const GLchar* fragSource;

	// Compile vertex shader
	std::ifstream vertSourceFile("shader.vert");
	std::stringstream vertSourceBuff;
	while (vertSourceFile.good()) {
		vertSourceFile.getline(str, MAX_LEN);
		vertSourceBuff << str << std::endl;
	}
	vertSourceFile.close();
	shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	vertSource = _strdup(vertSourceBuff.str().c_str());
	glShaderSource(shaderVertex, 1, &vertSource, 0);
	glCompileShader(shaderVertex);
	glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling vertex shader: ";
		glGetShaderiv(shaderVertex, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetShaderInfoLog(shaderVertex, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		glDeleteShader(shaderVertex);
		exit(EXIT_FAILURE);
	}
	else 
	{
		std::cout << "Vertex shader compiled successfully!" << std::endl;
	}

	// Compile fragment shader
	std::ifstream fragSourceFile("shader.frag");
	std::stringstream fragSourceBuff;
	while (fragSourceFile.good()) {
		fragSourceFile.getline(str, MAX_LEN);
		fragSourceBuff << str << std::endl;
	}
	fragSourceFile.close();
	shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	fragSource = _strdup(fragSourceBuff.str().c_str());
	glShaderSource(shaderFragment, 1, &fragSource, 0);
	glCompileShader(shaderFragment);
	glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling fragment shader: ";
		glGetShaderiv(shaderFragment, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetShaderInfoLog(shaderFragment, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		glDeleteShader(shaderFragment);
		glDeleteShader(shaderVertex);
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Fragment shader compiled successfully!" << std::endl;
	}
	
	// Create program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderVertex);
	glAttachShader(shaderProgram, shaderFragment);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling fragment shader: ";
		glGetProgramiv(shaderFragment, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetProgramInfoLog(shaderFragment, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		glDeleteProgram(shaderProgram);
		glDeleteShader(shaderFragment);
		glDeleteShader(shaderVertex);
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Shader program linked successfully!" << std::endl;
	}
}

void setupBuffers()
{
	glGenBuffers(1, &gVertexBuffer);
}

void useObject()
{
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
	glUseProgram(shaderProgram);
}

void renderStep()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	glVertexAttribPointer(0, NUM_DIMENSIONS, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
	glFlush();
}

int main() 
{
	// Setup opengl and compile shaders
	setupOpenGL();
	compileShader();
	setupBuffers();

	// Set object
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
	glDeleteProgram(shaderProgram);
	glDeleteShader(shaderFragment);
	glDeleteShader(shaderVertex);

	// Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}