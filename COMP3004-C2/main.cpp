//
//  main.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 19/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "utils.h"
#include "SceneObject.h"
#include "Skybox.h"

using namespace glm;
using namespace std;

void init();
void render(int frame, vector<SceneObject *> sceneObjects, vector<glm::vec3> sceneLights, GLuint shader);
void GLFWCALL keyHandler(int key, int action);
void checkKeyHolds();
void resetCameraPos1();

bool onTour = false;

struct Camera {
	vec3 location;
	vec3 focus;
	vec3 up;
	vec3 movement;
	float rotation;
	float rotationSpeed;
};
struct Camera camera;

int main(int argc, const char * argv[]) {
	int running = GL_TRUE;
	init();
	
	vector<GLint> shaders;
	shaders.push_back(setupShader("VertexShader.vert", GL_VERTEX_SHADER));
	shaders.push_back(setupShader("FragmentShader.frag", GL_FRAGMENT_SHADER));
	
	vector<vec3> sceneLights;
	vector<SceneObject *> sceneObjects;
	
	GLuint shader = linkShaders(shaders);
	glUseProgram(shader);
	check("Setup Shaders");
	
	SceneObject *landscape = new SceneObject("mars-landscape.obj", "");
	landscape->buffer();
	sceneObjects.push_back(landscape);
	
//	SceneObject *plane = new SceneObject("teapot.obj", "");
//	plane->buffer();
//	sceneObjects.push_back(plane);
	
	Skybox *skybox = new Skybox("mars");
	skybox->buffer();
	sceneObjects.push_back(skybox);
	
	vec3 sun = vec3(0.0, 100.0, 0.0);
	sceneLights.push_back(sun);
	
	int i = 0;
	static double t0 = glfwGetTime();
	static double fps = 60.0;
	while (running) {
		if ((glfwGetTime() - t0) > (1.0/fps)) {
			t0 = glfwGetTime();
			i++;
		}
		
		checkKeyHolds();
		
		render(i, sceneObjects, sceneLights, shader);
		
		glFlush();
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
    return 0;
}

void init() {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	/*
	 We must ask for version 3.2 on Mac OS. That is the highest supported, but does not
	 appear to be the default. If we don't do this, the GLSL 1.50 features will not work.
	 */
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	if (!glfwOpenWindow(1280, 800, 0, 0, 0, 0, 0, 1, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glewExperimental = GL_TRUE;
	glewInit(); // generates error
	glGetError(); // clear the error
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	glfwSetKeyCallback(keyHandler);
	
	resetCameraPos1();
}

void render(int frame, vector<SceneObject *> sceneObjects, vector<glm::vec3> sceneLights, GLuint shader) {
	
	mat4 perspective = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 10000.0f);
	mat4 model = mat4(1.0f);
	
	camera.location = camera.location + camera.movement;
	vec3 centre = vec3(cosf(camera.rotation/180 * 3.141592654f), 0.0, sinf(camera.rotation/180 * 3.141592654f)) + camera.location;
	camera.rotation += camera.rotationSpeed;
	
	mat4 view = lookAt(camera.location, centre, camera.up);
	view = rotate(view, camera.rotation, vec3(0.0, 1.0, 0.0));
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "perspective"), 1, GL_FALSE, value_ptr(perspective));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(view));
	
	glUniform1i(glGetUniformLocation(shader, "lightcount"), (GLint)sceneLights.size());
	glUniform3fv(glGetUniformLocation(shader, "lightpos"), sizeof(glm::vec3), &sceneLights[0][0]);
	glUniform3fv(glGetUniformLocation(shader, "camerapos"), 1, value_ptr(camera.location));
	
	for (int i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->render();
	}
}

#define TEST_KEY(k) (key == k || key == (k + 32))
#define GLFW_TEST_KEY(k) (glfwGetKey(k) == GLFW_PRESS)
void GLFWCALL keyHandler(int key, int action) {
	if (action != GLFW_PRESS) return;
	
	if (TEST_KEY('Q') || glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS) {
		glfwTerminate();
		exit(EXIT_SUCCESS);
	} else if (TEST_KEY('E')) {
		// Exit the tour
		onTour = false;
	}
	
	if (onTour) {
		// Nothing else should work whilst on tour
		return;
	}
	
	// Locations
	if (TEST_KEY('P')) {
		// Move to screenshot location
		
	} else if (TEST_KEY('Y')) {
		// Move to alternative location 1
		
	} else if (TEST_KEY('U')) {
		// Move to alternative location 2
		
	} else if (TEST_KEY('T')) {
		// Start tour, ignore other key presses except exit
		onTour = true;
	} else if (TEST_KEY('R')) {
		// Reset all animation
	}
}

void checkKeyHolds() {
	if (onTour) {
		return;
	}
	
	camera.movement = vec3(0.0, 0.0, 0.0);
	camera.rotationSpeed = 0.0;
	
#define TURNING_SPEED 2.0
#define MOVEMENT_SPEED 1.0
	
	// Movement
	if (GLFW_TEST_KEY(GLFW_KEY_LEFT)) {
		// Turn left
		camera.rotationSpeed = -TURNING_SPEED;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_RIGHT)) {
		// Turn right
		camera.rotationSpeed = TURNING_SPEED;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_UP)) {
		// Increase forward speed of camera
		camera.movement.x = cosf(camera.rotation / 180 * 3.141592654f) * MOVEMENT_SPEED;
		camera.movement.z = sinf(camera.rotation / 180 * 3.141592654f) * MOVEMENT_SPEED;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_DOWN)) {
		// Decrease forward speed of camera (no reverse!)
		camera.movement.x = cosf(camera.rotation / 180 * 3.141592654f) * -MOVEMENT_SPEED;
		camera.movement.z = sinf(camera.rotation / 180 * 3.141592654f) * -MOVEMENT_SPEED;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_SPACE)) {
		// Stop
		camera.movement = vec3(0.0, 0.0, 0.0);
		camera.rotationSpeed = 0.0;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_PAGEUP) || (GLFW_TEST_KEY(GLFW_KEY_SPECIAL) && GLFW_TEST_KEY(GLFW_KEY_UP))) {
		// Increase elevation of camera
		camera.movement.y = MOVEMENT_SPEED;
	}
	if (GLFW_TEST_KEY(GLFW_KEY_PAGEDOWN) || (GLFW_TEST_KEY(GLFW_KEY_SPECIAL) && GLFW_TEST_KEY(GLFW_KEY_DOWN))) {
		// Decrease elevation of camera
		camera.movement.y = -MOVEMENT_SPEED;
	}
	
#undef TURNING_SPEED
#undef MOVEMENT_SPEED
}
#undef TEST_KEY
#undef GLFW_TEST_KEY

void resetCameraPos1() {
	camera.location =	vec3(0.0, 20.0, 3.0);
	camera.focus =		vec3(0.0, 0.0, 0.0);
	camera.up =			vec3(0.0, 1.0, 0.0);
	camera.movement =	vec3(0.0, 0.0, 0.0);
	camera.rotation =	0.0;
	camera.rotationSpeed = 0.0;
}
