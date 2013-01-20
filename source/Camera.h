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
	float speed;
	float mouseSpeed;
	
	float forwardSpeed;
	
	double lastLoopTime;
	
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	
public:
	Camera();
	vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float fieldOfView;
	
	void computeMatricesFromInputs(bool ignoreInputs);
	mat4 getProjectionMatrix();
	mat4 getViewMatrix();
	vec3 getPosition();
	
	void resetToPosition0();
	void resetToPosition1();
	void resetToPosition2();
};


#endif /* defined(__COMP3004_C2__controls__) */
