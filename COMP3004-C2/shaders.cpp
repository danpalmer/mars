//
//  shaders.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 19/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "shaders.h"

#include "utils.h"

char *_fileToBuffer(char const *filename) {
	FILE *fptr;
	long length;
	char *buf;
	fptr = fopen(filename, "rb");
	if (!fptr) {
		fprintf(stderr, "failed to open %s\n", filename);
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char *)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}

GLuint _loadShader(char const *filename, int type) {
	char error[1000];
	GLint Result = GL_FALSE;
    int InfoLogLength;
	
	GLuint shader;
	GLchar *source = _fileToBuffer(filename);
	
	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char **)&source, NULL);
	glCompileShader(shader);
	
	// Check Vertex Shader
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    glGetShaderInfoLog(shader, InfoLogLength, NULL, error);
	if (InfoLogLength > 0) {
		std::cout << "Error Compiling Shader: " << error << "\n";
	}
	
	return shader;
}

GLint setupShader(char const *filename, GLint type) {
	GLint shader = _loadShader(filename, type);
	
	check("Loaded Shader");
	return shader;
}

GLint linkShaders(std::vector<GLint> shaders) {
	GLint program = glCreateProgram();
	for (int i = 0; i < shaders.size(); i++) {
		glAttachShader(program, shaders[i]);
	}
	
	glLinkProgram(program);
	
	char error[1000]; int errlen = 0;
	glGetProgramInfoLog(program, 0, &errlen, error);
	if (errlen > 0) {
		std::cout << "Error Linking Shaders: " << error << "\n";
	}
	
	check("Linked Shaders");
	return program;
}
