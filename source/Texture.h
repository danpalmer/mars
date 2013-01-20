//
//  Texture.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 24/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__Texture__
#define __COMP3004_C2__Texture__

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>


class Texture {
public:
	GLuint textureID;
	
	Texture(char const *filename, int flags, bool repeat);
	
	void bind();
	void unbind();

};

#endif /* defined(__COMP3004_C2__Texture__) */
