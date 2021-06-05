#version 330

// Attributes
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec3 aNormal;

// Transform uniforms
uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;

// Lighting uniforms
uniform vec3 ambientLight;
uniform vec3 directionalLightColor;
uniform vec3 directionalLightVector;

// Outputs
out vec3 thePos;
out vec2 theTexCoord;
out vec3 theNormal;
out vec3 theLight;

// Intermediates
vec4 viewSpacePos;

void main()
{
	// Model view and normal matrix
	mat4 modelView = camera * transform;
	mat3 normalMat = mat3(transpose(inverse(modelView)));

	// Lighting
	vec3 transNormal = normalMat * aNormal;
	float directionalValue = max(dot(transNormal, directionalLightVector), 0.0);
	vec3 directionalLight = directionalLightColor * directionalValue;

	// Outputs
	viewSpacePos = transform * vec4(aPosition, 1.0);
	gl_Position = projection * camera * viewSpacePos;
	thePos = vec3(viewSpacePos);
	theTexCoord = aTexcoord;
	theNormal = transNormal;
	theLight = ambientLight + directionalLight;
}