#version 330

in vec2 theTexCoord;
in vec3 theLight;

vec3 theColor;

uniform sampler2D tex;

void main()
{
	theColor = vec3(texture(tex, theTexCoord));
	gl_FragColor = vec4(theColor * theLight, 1.0);
}