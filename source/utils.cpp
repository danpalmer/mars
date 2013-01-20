//
//  utils.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 19/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "utils.h"

#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glfw.h>

void check(char const *where) { // Function to check OpenGL error status
	char *what;
	int err = glGetError(); //0 means no error
	if(!err)
		return;
	if(err == GL_INVALID_ENUM)
		what = (char *)"GL_INVALID_ENUM";
	else if(err == GL_INVALID_VALUE)
		what = (char *)"GL_INVALID_VALUE";
	else if(err == GL_INVALID_OPERATION)
		what = (char *)"GL_INVALID_OPERATION";
	else if(err == GL_INVALID_FRAMEBUFFER_OPERATION)
		what = (char *)"GL_INVALID_FRAMEBUFFER_OPERATION";
	else if(err == GL_OUT_OF_MEMORY)
		what = (char *)"GL_OUT_OF_MEMORY";
	else
		what = (char *)"Unknown Error";
	fprintf(stderr, "Error (%d) %s  at %s\n", err, what, where);
	exit(1);
}