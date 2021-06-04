#version 330

// Attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

// Outputs
out vec3 theColor;
out vec3 theLight;

void main()
{
	// Model view and normal matrix
	mat4 modelView = camera * transform;
	mat3 normalMat = mat3(transpose(inverse(modelView)));

	// Lighting
	vec3 ambientLight = vec3(0.2, 0.2, 0.2);
    vec3 directionalLightColor = vec3(1, 1, 1);
    vec3 directionalVector = normalize(vec3(0.85, 0.8, 0.75));
	vec3 transNormal = normalMat * aNormal;
	float directionalValue = max(dot(transNormal, directionalVector), 0.0);
	vec3 directionalLight = directionalLightColor * directionalValue;

	// Outputs
	gl_Position = projection * modelView * vec4(aPosition, 1.0);
	theColor = aColor;
	theLight = ambientLight + directionalLight;
}