//
//  shaders.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 19/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__shaders__
#define __COMP3004_C2__shaders__

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>

GLint setupShader(char const *filename, GLint type);
GLint linkShaders(std::vector<GLint> shaders);

#endif /* defined(__COMP3004_C2__shaders__) */
