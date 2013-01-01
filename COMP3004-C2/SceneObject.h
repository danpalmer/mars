//
//  SceneObject.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 20/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__SceneObject__
#define __COMP3004_C2__SceneObject__

#include "Texture.h"

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

typedef struct {
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec4 lighting; // Ambient, Diffuse, Specular, Shininess
	GLuint texture;
} Vertex;

class SceneObject {
public:
	std::vector<Vertex> vertices;
	std::vector<Texture *> textures;
	GLuint vertexBuffer, vertexArray;
	bool smooth, wireframe;
	
	SceneObject();
    SceneObject(char const *objFilename, char const *mtlFilename);
	
	void buffer();
	void bind();
	void unbind();
	virtual void render();

private:
	void _loadOBJ(char const *filename);
};

#endif /* defined(__COMP3004_C2__SceneObject__) */
