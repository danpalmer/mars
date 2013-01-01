//
//  SceneObject.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 20/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "SceneObject.h"

#include <fstream>
using namespace std;
using namespace glm;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

SceneObject::SceneObject() {
	smooth = true;
	wireframe = false;
}

SceneObject::SceneObject(char const *objFilename, char const *mtlFilename) {
    this->_loadOBJ(objFilename);
	smooth = false;
	wireframe = false;
	check("Created Object");
}

void SceneObject::buffer() {
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	check("Buffered Object");
}

void SceneObject::bind() {
	glBindVertexArray(vertexArray);
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
	
	glBindVertexArray(0);
	
	check("Unbound SceneObject");
}

void SceneObject::render() {
	bind();
	
	if (smooth) {
		glEnable(GL_SMOOTH);
	} else {
		glEnable(GL_FLAT);
	}
	glGetError();
	
	glDrawArrays(wireframe ? GL_LINES : GL_TRIANGLES, 0, (GLsizei)vertices.size());
	check("Rendered SceneObject");
	
	unbind();
}

typedef struct {
	unsigned int vertexIndex[3] = {0}, uvIndex[3] = {0}, normalIndex[3] = {0};
	bool uvs, normals;
} Face;

void SceneObject::_loadOBJ(const char *filename) {
	
	ifstream fin;
	fin.open(filename);
	if (!fin.good()) {
		fprintf(stderr, "Failed to read object file %s", filename);
		exit(EXIT_FAILURE);
		return;
	}
	
	const int MAX_CHARS_PER_LINE = 128;
	
	vector<vec3> verts;
	vector<vec2> uvs;
	vector<vec3> normals;
	vector<Face> faces;
	
	while (!fin.eof()) {
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);
		
		string line = buf;
		
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
			Face face;
			
			int matches = sscanf(buf, "f %d %d %d", &face.vertexIndex[0], &face.vertexIndex[1], &face.vertexIndex[2]);
			if (matches == 3) {
				face.normals = false;
				face.uvs = false;
				faces.push_back(face);
				continue;
			}
			
			matches = sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d", &face.vertexIndex[0], &face.uvIndex[0], &face.normalIndex[0], &face.vertexIndex[1], &face.uvIndex[1], &face.normalIndex[1], &face.vertexIndex[2], &face.uvIndex[2], &face.normalIndex[2]);
			if (matches == 9) {
				face.normals = true;
				face.uvs = true;
				faces.push_back(face);
				continue;
			}
			
			matches = sscanf(buf, "f %d/%d %d/%d %d/%d", &face.vertexIndex[0], &face.uvIndex[0], &face.vertexIndex[1], &face.uvIndex[1], &face.vertexIndex[2], &face.uvIndex[2]);
			if (matches == 6) {
				face.normals = false;
				face.uvs = true;
				faces.push_back(face);
				continue;
			}
			
			matches = sscanf(buf, "f %d//%d %d//%d %d//%d", &face.vertexIndex[0], &face.normalIndex[0], &face.vertexIndex[1], &face.normalIndex[1], &face.vertexIndex[2], &face.normalIndex[2]);
			if (matches > 0) {
				face.normals = true;
				face.uvs = false;
				faces.push_back(face);
				continue;
			}
			
			cout << "Failed to parse face" << endl;
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
	
#if DEBUG
	cout << "\tRead " << verts.size() << " vertices" << endl;
	cout << "\tRead " << uvs.size() << " UVs" << endl;
	cout << "\tRead " << normals.size() << " normals" << endl;
	cout << "\tRead " << faces.size() << " faces" << endl;
#endif
	
	for (unsigned int i = 0; i < faces.size(); i++) {
		Face face = faces[i];
		for (int v = 0; v < 3; v++) {
			Vertex vertex;
			
			vec3 vert = verts[face.vertexIndex[v] - 1];
			vertex.position = glm::vec4(vert, 1.0);
			
			if (face.uvs) {
				vec2 uv = uvs[face.uvIndex[v] - 1];
				vertex.texcoords = uv;
			}
			
			if (face.normals) {
				vec3 normal = normals[face.normalIndex[v] - 1];
				vertex.normal = normal;
			}
			
			vertex.colour = glm::vec4(0.5940, 0.2100, 0.0588, 1);
			vertex.lighting = glm::vec4(0.2, 0.8, 1.0, 100.0);
			vertex.texture = -1;
			
			vertices.push_back(vertex);
		}
	}
	
	cout << "Read object " << filename << endl;
	fin.close();
}
