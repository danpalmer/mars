//
//  controls.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 03/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#include "controls.h"

#define MOUSE_CONTROL 0
#define TURN_SPEED 0.05

Camera::Camera() {
	resetToPosition1();
}

void Camera::computeMatricesFromInputs(bool ignoreInputs) {
	
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastLoopTime);
	lastLoopTime = currentTime;
	
	int width, height;
	glfwGetWindowSize(&width, &height);
#if MOUSE_CONTROL
	int xpos, ypos;
	glfwGetMousePos(&xpos, &ypos);
	glfwSetMousePos(width / 2, height / 2);
	
	if (!ignoreInputs) {
		horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xpos);
		verticalAngle	+= mouseSpeed * deltaTime * float(height / 2 - ypos);
	}
#else
	if (!ignoreInputs) {
		verticalAngle = 0;
	}
#endif
	
	vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
	    sin(verticalAngle),
	    cos(verticalAngle) * cos(horizontalAngle)
	);
	
	vec3 right = vec3(
		sin(horizontalAngle - M_PI / 2.0f),
		0,
		cos(horizontalAngle - M_PI / 2.0f)
	);
	
	vec3 up = cross(right, direction);
	
	if (!ignoreInputs) {
		if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS){
			position += direction * deltaTime * speed;
		}
		
		if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS){
			position -= direction * deltaTime * speed;
		}
		
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS){
#if MOUSE_CONTROL
			position += right * deltaTime * speed;
#else
			horizontalAngle -= TURN_SPEED;
#endif
		}
		
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS){
#if MOUSE_CONTROL
			position -= right * deltaTime * speed;
#else
			horizontalAngle += TURN_SPEED;
#endif
		}
	}
	
	float FoV = fieldOfView;
	ProjectionMatrix = glm::perspective(FoV, float(width / height), 0.1f, 15000.0f);
	ViewMatrix = glm::lookAt(position, position + direction, up);
}

mat4 Camera::getProjectionMatrix() {
	return ProjectionMatrix;
}

mat4 Camera::getViewMatrix() {
	return ViewMatrix;
}

vec3 Camera::getPosition() {
	return position;
}

void Camera::resetToPosition0() {
	position = vec3(-575,550,-3881);
	horizontalAngle = 0.5;
#if MOUSE_CONTROL
	verticalAngle = -0.01;
#else
	verticalAngle = 0;
#endif
	fieldOfView = 45.0f;
}

void Camera::resetToPosition1() {
	position = vec3(150,1000,-5000);
	horizontalAngle = 0;
#if MOUSE_CONTROL
	verticalAngle = -0.25;
#else
	verticalAngle = 0;
#endif
	fieldOfView = 45.0f;
}

void Camera::resetToPosition2() {
	horizontalAngle = 2.628726;
#if MOUSE_CONTROL
	position = vec3(-1018.718994, 1743.249878, 1513.233521);
	verticalAngle = -0.333765;
#else
	position = vec3(-1018.718994, 1043.249878, 1513.233521);
	verticalAngle = 0;
#endif
	fieldOfView = 45;
}
