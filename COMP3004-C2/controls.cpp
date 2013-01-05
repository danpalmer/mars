//
//  controls.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 03/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#include "controls.h"

void Camera::computeMatricesFromInputs() {
	
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastLoopTime);
	lastLoopTime = currentTime;
	
	int xpos, ypos, width, height;
	glfwGetMousePos(&xpos, &ypos);
	glfwGetWindowSize(&width, &height);
	glfwSetMousePos(width / 2, height / 2);
	
	horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xpos);
	verticalAngle	+= mouseSpeed * deltaTime * float(height / 2 - ypos);
	
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
	
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}

	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}

	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}

	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	
	float FoV = fieldOfView - 5 * glfwGetMouseWheel();
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
