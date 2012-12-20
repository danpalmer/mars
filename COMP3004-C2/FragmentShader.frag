#version 330 core

in Attributes {
	vec4 position;
	vec4 colour;
	vec3 normal;
	vec2 texcoords;
	vec3 lighting;
} attribs;

out vec4 colour;

void main(void) {
	
	// Shut up GLSL compiler
	attribs.position;
	
    colour = vec4(1,0,0,1);
}
