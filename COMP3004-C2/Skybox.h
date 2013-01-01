//
//  Skybox.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 24/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__Skybox__
#define __COMP3004_C2__Skybox__

#include <iostream>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "SceneObject.h"

class Skybox : public SceneObject {	
public:
	/*
	 Textures for the Skybox are expected in the format:
		<name>_<direction>.tga
	 Where <direction> is one of {top, bottom, left, right, front, back}
	 */
	Skybox(char const *textureName);
	Skybox(float size, char const *textureName);
	
	void render();
	
private:
	void init(float size, char const *textureName);
};

#endif /* defined(__COMP3004_C2__Skybox__) */
