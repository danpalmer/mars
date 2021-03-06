#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoords;
layout(location = 4) in vec4 lighting;
layout(location = 5) in int surfaceType;

uniform mat4 perspective;
uniform mat4 model;
uniform mat4 view;
uniform int surface;

const int MAXLIGHTS = 4;
uniform int lightcount;
uniform vec3 lightpos[MAXLIGHTS];
uniform vec3 camerapos;

uniform sampler2D texture;

out Attributes {
	vec4 position;
	vec4 colour;
	vec3 normal;
	vec2 texcoords;
	vec4 lighting;
} attribs;

flat out int out_surfaceType;

void main(void) {
	
	mat4 modelView = view * model;
	attribs.position = perspective * modelView * position;
	
	attribs.colour = colour;
	attribs.normal = normal;
	attribs.texcoords = texcoords;
	attribs.lighting = lighting;
	out_surfaceType = surface;
	
	gl_Position = attribs.position;
}
