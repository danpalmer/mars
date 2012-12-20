#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoords;
layout(location = 4) in vec3 lighting;

out Attributes {
	vec4 position;
	vec4 colour;
	vec3 normal;
	vec2 texcoords;
	vec3 lighting;
} attribs;

void main(void) {
	
	attribs.position = position;
	attribs.colour = colour;
	attribs.normal = normal;
	attribs.texcoords = texcoords;
	attribs.lighting = lighting;
	
	gl_Position = position;
}
