//
//  SceneObject.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 20/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__SceneObject__
#define __COMP3004_C2__SceneObject__

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
} Vertex;

class SceneObject
{

public:
    SceneObject(char const *objFilename, char const *mtlFilename);
    ~SceneObject();
	
	void buffer();
	void bind();
	void unbind();
	void render();

private:
	std::vector<Vertex> vertices;
	GLuint vertexBuffer;
	bool smooth, wireframe;
	
	void _loadOBJ(char const *filename);
};

#endif /* defined(__COMP3004_C2__SceneObject__) */
