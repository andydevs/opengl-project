#version 330

// Attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

// Uniforms
uniform mat4 transform;

// Outputs
out vec3 theColor;

void main()
{
	gl_Position = transform * vec4(aPosition, 1.0);
	theColor = aColor;
}