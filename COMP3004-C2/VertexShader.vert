#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoords;
layout(location = 4) in vec4 lighting;
layout(location = 5) in int texture;

uniform mat4 perspective;
uniform mat4 model;
uniform mat4 view;

const int MAXLIGHTS = 4;
uniform int lightcount;
uniform vec3 lightpos[MAXLIGHTS];
uniform vec3 camerapos;

const int MAXTEXTURES = 6;
uniform sampler2D textures[MAXTEXTURES];

out Attributes {
	vec4 position;
	vec4 colour;
	vec3 normal;
	vec2 texcoords;
	vec4 lighting;
	flat int texture;
} attribs;

//out int out_lightcount;
//out vec3 out_lightpos[MAXLIGHTS];

void main(void) {
	
	mat4 modelView = model * view;
	attribs.position = perspective * modelView * position;
	
	attribs.colour = colour;
	attribs.normal = normal;
	attribs.texcoords = texcoords;
	attribs.lighting = lighting;
	attribs.texture = texture;
	
	gl_Position = attribs.position;
	
	// Pass through lights
//	out_lightcount = lightcount;
//	out_lightpos = lightpos;
}
