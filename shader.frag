#version 330

in vec3 theColor;
in vec3 theLight;

void main()
{
	gl_FragColor = vec4(theColor * theLight, 1.0);
}