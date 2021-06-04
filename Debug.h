#pragma once
#include <GL/glew.h>
#include <iostream>

#define PRINT_CASE(x) case x: std::cout << #x; break;

#define GL_SAFE_CALL(x)\
	while(glGetError() != GL_NO_ERROR);\
	x;\
	while (GLenum error = glGetError()) {\
		std::cout << "OpenGL Error ";\
		switch (error) {\
		PRINT_CASE(GL_INVALID_OPERATION)\
		PRINT_CASE(GL_STACK_UNDERFLOW)\
		PRINT_CASE(GL_STACK_OVERFLOW)\
		PRINT_CASE(GL_INVALID_VALUE)\
		PRINT_CASE(GL_OUT_OF_MEMORY)\
		PRINT_CASE(GL_INVALID_ENUM)\
		default: std::cout << error; break;\
		}\
		std::cout << " in " << #x << std::endl; \
		__debugbreak();\
	}