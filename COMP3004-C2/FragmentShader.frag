#version 330 core

in Attributes {
	vec4 position;
	vec4 colour;
	vec3 normal;
	vec2 texcoords;
	vec4 lighting;
} attribs;

const int MAXLIGHTS = 4;
uniform int lightcount;
uniform vec3 lightpos[MAXLIGHTS];
uniform vec3 camerapos;

uniform sampler2D texture;

out vec4 colour;

void main(void) {


	vec4 diffuse = vec4(attribs.lighting.y, attribs.lighting.y, attribs.lighting.y, 1.0);
	vec4 specular = vec4(attribs.lighting.z, attribs.lighting.z, attribs.lighting.z, 1.0);
	float shininess = attribs.lighting.w;
	float ambient = attribs.lighting.x;
	
	//	for (int i = 0; i < lightcount; i++) {
	vec3 light = lightpos[0];
	vec3 L = normalize(light - attribs.position.xyz);
	vec3 E = normalize(camerapos - attribs.position.xyz);
	vec3 R = normalize(-reflect(L, attribs.normal.xyz));
	
	vec4 Idiff = diffuse * max(dot(attribs.normal.xyz,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);
	vec4 Ispec = specular * pow(max(dot(R,E), 0.0), 1.0 * shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);
	
	//		colour += (ambient + Ispec + Idiff);
	//	}

	// Textures are exact blue, would be better to have a flag in the attribs
	if (attribs.colour == vec4(0.0, 0.0, 1.0, 1.0)) {
		colour = vec4(texture(texture, attribs.texcoords).rgb, 1.0);
	} else {
		colour = attribs.colour * (ambient + Ispec + Idiff);//colour;
	}
}