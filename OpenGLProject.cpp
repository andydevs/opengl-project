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
#define NUM_VERTICES 3
#define NUM_POSITION_DIMENSIONS 2
const float geometry[NUM_VERTICES * NUM_POSITION_DIMENSIONS] = {
	-0.5f, -0.5f,
	0.0, 0.5,
	0.5, -0.5
};
#define NUM_COLOR_DIMENSIONS 3
const float color[NUM_VERTICES * NUM_COLOR_DIMENSIONS] = {
	0.0, 0.0, 1.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
};

// Resources
GLFWwindow* window;
GLuint gPositionBuffer;
GLuint gColorBuffer;
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

GLint compileShader(const char* sourceFileName, GLenum shaderType)
{
	// Parameters n soch
	const int MAX_LEN = 2048;
	const GLchar* source;
	GLint shaderHandle;
	GLint isGood;
	GLint maxlen;
	char str[MAX_LEN];

	// Read file contents
	std::ifstream sourceFile(sourceFileName);
	std::stringstream sourceBuff;
	while (sourceFile.good()) {
		sourceFile.getline(str, MAX_LEN);
		sourceBuff << str << std::endl;
	}
	sourceFile.close();
	source = _strdup(sourceBuff.str().c_str());

	// Compile shader
	shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, &source, 0);
	glCompileShader(shaderHandle);

	// Check errors
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling shader " << sourceFileName << std::endl;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetShaderInfoLog(shaderHandle, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		exit(EXIT_FAILURE); // TODO: DONT DO THIS HORRIBLE THING
	}
	else 
	{
		std::cout << "Successfuly compiled shader " << sourceFileName << std::endl;
	}

	// Return compiled shader handle
	return shaderHandle;
}

GLint linkShaderProgram(GLint shaderVertex, GLint shaderFragment) 
{
	// Buffers n shoch
	GLint isGood;
	GLint maxlen;
	
	// Create program and attach shaders and link
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderVertex);
	glAttachShader(shaderProgram, shaderFragment);
	glLinkProgram(shaderProgram);

	// Handle errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isGood);
	if (isGood == GL_FALSE)
	{
		std::cout << "Error compiling fragment shader: ";
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxlen);
		std::vector<GLchar> infolog(maxlen);
		glGetProgramInfoLog(shaderProgram, maxlen, &maxlen, &infolog[0]);
		std::string infologstr(infolog.begin(), infolog.end());
		std::cout << infologstr << std::endl;
		exit(EXIT_FAILURE); // TODO: IT AINT COOL HERE EITHER
	}
	else
	{
		std::cout << "Shader program linked successfully!" << std::endl;
	}

	// Return shader program
	return shaderProgram;
}

void useObject()
{
	glBindBuffer(GL_ARRAY_BUFFER, gPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, gColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
	glUseProgram(shaderProgram);
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
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
	glFlush();
}

int main() 
{
	// Setup opengl
	setupOpenGL();

	// Compile and link shader program
	shaderVertex = compileShader("shader.vert", GL_VERTEX_SHADER);
	shaderFragment = compileShader("shader.frag", GL_FRAGMENT_SHADER);
	shaderProgram = linkShaderProgram(shaderVertex, shaderFragment);

	// Set up opengl buffers
	glGenBuffers(1, &gPositionBuffer);
	glGenBuffers(1, &gColorBuffer);

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