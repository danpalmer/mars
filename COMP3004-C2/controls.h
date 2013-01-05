//
//  controls.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 03/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__controls__
#define __COMP3004_C2__controls__

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glfw.h>

using namespace glm;

class Camera {
	glm::vec3 position = glm::vec3( 0, 0, 5 );
	
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float fieldOfView = 45.0f;
	
	float speed = 1000.0f;
	float mouseSpeed = 0.5f;
	
	double lastLoopTime = 0.0;
	
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	
public:
	void computeMatricesFromInputs();
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	vec3 getPosition();
};


#endif /* defined(__COMP3004_C2__controls__) */
