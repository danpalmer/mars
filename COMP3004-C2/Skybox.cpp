//
//  Skybox.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 24/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "Skybox.h"

#include "utils.h"
#include "Texture.h"

#define DEFAULT_SIZE (float)11000.0
#define PT (float)(size / 2.0)

using namespace glm;
using namespace std;

Vertex skyboxVertex(float x, float y, float z, int texture, float s, float t);

Skybox::Skybox(float size, char const *textureName) {
	init(size, textureName);
}

Skybox::Skybox(char const *textureName) {
	init(DEFAULT_SIZE, textureName);
}

void Skybox::render(GLint shader) {
	bind();
	
	int vertsPerTexture = (int)(vertices.size() / textures.size());
	for (int i = 0; i < textures.size(); i++) {
		textures[i]->bind();
		glDrawArrays(GL_TRIANGLES, i * vertsPerTexture, vertsPerTexture);
		textures[i]->unbind();
	}
	
	check("Rendered SceneObject");
	
	unbind();
}

void Skybox::init(float size, const char *textureName) {
	surfaceType = textured;
	string texture = textureName, face;
	
	face = "_front.tga";
	Texture *textureF = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex(-PT, -PT,  PT, 0, 1, 0));
	vertices.push_back(skyboxVertex(-PT,  PT,  PT, 0, 1, 1));
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 0, 0, 1));
	
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 0, 0, 1));
	vertices.push_back(skyboxVertex( PT, -PT,  PT, 0, 0, 0));
	vertices.push_back(skyboxVertex(-PT, -PT,  PT, 0, 1, 0));
	textures.push_back(textureF);
	
	face = "_back.tga";
	Texture *textureB = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex( PT,  PT, -PT, 1, 1, 1));
	vertices.push_back(skyboxVertex(-PT,  PT, -PT, 1, 0, 1));
	vertices.push_back(skyboxVertex(-PT, -PT, -PT, 1, 0, 0));
	
	vertices.push_back(skyboxVertex(-PT, -PT, -PT, 1, 0, 0));
	vertices.push_back(skyboxVertex( PT, -PT, -PT, 1, 1, 0));
	vertices.push_back(skyboxVertex( PT,  PT, -PT, 1, 1, 1));
	textures.push_back(textureB);

	face = "_top.tga";
	Texture *textureT = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex(-PT,  PT, -PT, 2, 0, 1));
	vertices.push_back(skyboxVertex( PT,  PT, -PT, 2, 0, 0));
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 2, 1, 0));
	
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 2, 1, 0));
	vertices.push_back(skyboxVertex(-PT,  PT,  PT, 2, 1, 1));
	vertices.push_back(skyboxVertex(-PT,  PT, -PT, 2, 0, 1));
	textures.push_back(textureT);

	face = "_bottom.tga";
	Texture *textureBt = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex( PT, -PT,  PT, 3, 1, 1));
	vertices.push_back(skyboxVertex( PT, -PT, -PT, 3, 0, 1));
	vertices.push_back(skyboxVertex(-PT, -PT, -PT, 3, 0, 0));
	
	vertices.push_back(skyboxVertex(-PT, -PT, -PT, 3, 0, 0));
	vertices.push_back(skyboxVertex(-PT, -PT,  PT, 3, 1, 0));
	vertices.push_back(skyboxVertex( PT, -PT,  PT, 3, 1, 1));
	textures.push_back(textureBt);

	face = "_left.tga";
	Texture *textureL = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex(-PT,  PT, -PT, 4, 1, 1));
	vertices.push_back(skyboxVertex(-PT,  PT,  PT, 4, 0, 1));
	vertices.push_back(skyboxVertex(-PT, -PT,  PT, 4, 0, 0));
	
	vertices.push_back(skyboxVertex(-PT, -PT,  PT, 4, 0, 0));
	vertices.push_back(skyboxVertex(-PT, -PT, -PT, 4, 1, 0));
	vertices.push_back(skyboxVertex(-PT,  PT, -PT, 4, 1, 1));
	textures.push_back(textureL);
	
	face = "_right.tga";
	Texture *textureR = new Texture((texture + face).c_str(), 0, false);
	vertices.push_back(skyboxVertex( PT, -PT, -PT, 5, 0, 0));
	vertices.push_back(skyboxVertex( PT, -PT,  PT, 5, 1, 0));
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 5, 1, 1));
	
	vertices.push_back(skyboxVertex( PT,  PT,  PT, 5, 1, 1));
	vertices.push_back(skyboxVertex( PT,  PT, -PT, 5, 0, 1));
	vertices.push_back(skyboxVertex( PT, -PT, -PT, 5, 0, 0));
	textures.push_back(textureR);
	
	check("Created Skybox");
}

Vertex skyboxVertex(float x, float y, float z, int texture, float s, float t) {
	Vertex v;
	v.position = vec4(x, y, z, 1.0);
	v.texcoords = vec2(s, t);
	v.colour = RED;
	return v;
}
