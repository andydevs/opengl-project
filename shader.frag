#version 330

// Inputs
in vec3 thePos;
in vec2 theTexCoord;
in vec3 theNormal;
in vec3 theLight;

// Internal variables
vec3 eye;
vec3 texColor;
float rimCont;
vec3 rimColor;

// Uniforms
uniform vec3 rimLight;
uniform sampler2D tex;

void main()
{
	// Texture coloring
	texColor = vec3(texture(tex, theTexCoord));

	// Rim Lighting
	eye = normalize(-thePos);
	rimCont = 1.0 - max(dot(eye, theNormal), 0.0);
	rimCont = smoothstep(0.8, 1.0, rimCont);
	rimColor = rimLight * rimCont;
	
	// Final color1
	gl_FragColor = vec4((rimColor + theLight) * texColor , 1.0);
}