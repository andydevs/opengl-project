#version 330

// Attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

// Uniforms
uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

// Outputs
out vec3 theColor;

void main()
{
	gl_Position = projection * camera * transform * vec4(aPosition, 1.0);
	theColor = aColor;
}