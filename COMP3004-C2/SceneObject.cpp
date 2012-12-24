//
//  SceneObject.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 20/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "SceneObject.h"

#include <fstream>
using std::ifstream;

using std::cout;
using std::endl;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

SceneObject::SceneObject(char const *objFilename, char const *mtlFilename) {
    this->_loadOBJ(objFilename);
	smooth = false;
	wireframe = false;
	check("Created Object");
}

void SceneObject::buffer() {
	glGenBuffers(1, &vertexBuffer);
	bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
}

void SceneObject::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
#define VERTEX_OFFSETOF(f) (GLvoid *)((char *)&(vertices[0].f) - (char *)&(vertices[0]))
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_OFFSETOF(position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_OFFSETOF(colour));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_OFFSETOF(normal));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_OFFSETOF(texcoords));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), VERTEX_OFFSETOF(lighting));
#undef VERTEX_OFFSETOF
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	
	check("Bound SceneObject");
}

void SceneObject::unbind() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	
	check("Unbound SceneObject");
}

void SceneObject::render() {
	bind();

	// TODO: Set smoothing.
	// glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
	glDrawArrays(wireframe ? GL_LINES : GL_TRIANGLES, 0, (GLsizei)vertices.size());
	check("Rendered SceneObject");
	
	unbind();
}

void SceneObject::_loadOBJ(const char *filename) {
	
	ifstream fin;
	fin.open(filename);
	if (!fin.good()) {
		fprintf(stderr, "Failed to read object file %s", filename);
		exit(EXIT_FAILURE);
		return;
	}
	
	const int MAX_CHARS_PER_LINE = 128;
	
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	std::vector<int> vertexIndices, uvIndices, normalIndices;
	
	while (!fin.eof()) {
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
		
#define STARTSWITH(str) (strncmp(str, buf, strlen(str)) == 0)
		if (STARTSWITH("vn ")) {
			// parse normal
			glm::vec3 normal;
			int matches = sscanf(buf, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			if (matches == 0) {
				cout << "Failed to parse normal" << endl;
				continue;
			}
			normals.push_back(normal);
			
		} else if (STARTSWITH("vt ")) {
			// parse texture coord
			glm::vec2 uv;
			int matches = sscanf(buf, "vt %f %f", &uv.x, &uv.y);
			if (matches == 0) {
				cout << "Failed to parse UVs" << endl;
				continue;
			}
			uvs.push_back(uv);
			
		} else if (STARTSWITH("v ")) {
			// parse vertex
			glm::vec3 vert;
			int matches = sscanf(buf, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			if (matches == 0) {
				cout << "Failed to parse vertex" << endl;
				continue;
			}
			verts.push_back(vert);
			
		} else if (STARTSWITH("f ")) {
			// parse faces
			unsigned int vertexIndex[3] = {0}, uvIndex[3] = {0}, normalIndex[3] = {0};
			int matches = sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches == 0) {
				matches = sscanf(buf, "f %d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
			}
			if (matches == 0) {
				matches = sscanf(buf, "f %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			}
			if (matches == 0) {
				cout << "Failed to parse face" << endl;
				continue;
			}
			
			vertexIndices.push_back(vertexIndex[0] - 1);
			vertexIndices.push_back(vertexIndex[1] - 1);
			vertexIndices.push_back(vertexIndex[2] - 1);
			uvIndices.push_back(uvIndex[0] - 1);
			uvIndices.push_back(uvIndex[1] - 1);
			uvIndices.push_back(uvIndex[2] - 1);
			normalIndices.push_back(normalIndex[0] - 1);
			normalIndices.push_back(normalIndex[1] - 1);
			normalIndices.push_back(normalIndex[2] - 1);
			
		} else if (STARTSWITH("mtllib ")) {
			// parse the mtl file name
			
		} else if (STARTSWITH("usemtl ")) {
			// parse the material name
			
		} else if (STARTSWITH("#")) {
			// Comment - ignore
			continue;
		} else if (STARTSWITH("s ")) {
			// Smooth
			int match = sscanf(buf, "s 1");
			if (match == 1) {
				smooth = true;
			} else {
				match = sscanf(buf, "s 0");
				if (match == 0) {
					smooth = false;
				}
			}
			
		} else if (STARTSWITH("o ") || STARTSWITH("g ")) {
			// Named group - ignore
			continue;
		} else if (strlen(buf) == 0) {
			continue;
		} else {
			cout << "Unrecognized OBJ token: " << buf << endl;
		}
#undef STARTSWITH
	}
	
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		Vertex vertex;
		
		glm::vec3 vert = verts[vertexIndex];
		vertex.position = glm::vec4(vert, 1.0);
		
		glm::vec2 uv = uvs[vertexIndex];
		vertex.texcoords = uv;
		
		glm::vec3 normal = normals[vertexIndex];
		vertex.normal = normal;

		vertex.colour = RED;
		
		vertex.lighting = glm::vec4(0.2, 0.8, 1.0, 100.0);
		
		vertices.push_back(vertex);
	}
	
#if DEBUG
	cout << "\tRead " << verts.size() << " vertices" << endl;
	cout << "\tRead " << uvs.size() << " UVs" << endl;
	cout << "\tRead " << normals.size() << " normals" << endl;
#endif
	
	cout << "Read object " << filename << endl;
	fin.close();
}