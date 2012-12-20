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

using namespace glm;

void init();
void render(int frame, std::vector<SceneObject *>);
void GLFWCALL keyHandler(int key, int action);

GLuint vao;
bool onTour = false;

int main(int argc, const char * argv[]) {
	int running = GL_TRUE;
	init();
	
	std::vector<GLint> shaders;
	shaders.push_back(setupShader("VertexShader.vert", GL_VERTEX_SHADER));
	shaders.push_back(setupShader("FragmentShader.frag", GL_FRAGMENT_SHADER));
	
	GLint shader = linkShaders(shaders);
	check("Setup Shaders");
	glUseProgram(shader);
	
	SceneObject *ship = new SceneObject("cube.obj", "");
	ship->buffer();
	
	mat4 perspective = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
	mat4 model = mat4(1.0f);
	
	vec3 cameraLocation =	vec3(0.0, 1.0, 3.0);
	vec3 cameraFocus =		vec3(0.0, 0.0, 0.0);
	vec3 cameraUp =			vec3(0.0, 1.0, 0.0);
	mat4 camera = lookAt(cameraLocation, cameraFocus, cameraUp);
	
	std::vector<SceneObject *> sceneObjects;
	sceneObjects.push_back(ship);
	
	int i = 0;
	static double t0 = glfwGetTime();
	static double fps = 60.0;
	while (running) {
		if ((glfwGetTime() - t0) > (1.0/fps)) {
			t0 = glfwGetTime();
			i++;
		}
		
		render(i, sceneObjects);
		
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
	
	if (!glfwOpenWindow(800, 800, 0, 0, 0, 0, 0, 1, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glewExperimental = GL_TRUE;
	glewInit(); // generates error
	glGetError(); // clear the error
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glfwSetKeyCallback(keyHandler);
}

void render(int frame, std::vector<SceneObject *> sceneObjects) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
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
	}
	
	// Movement
	if (GLFW_TEST_KEY(GLFW_KEY_LEFT)) {
		// Move left
	} else if (GLFW_TEST_KEY(GLFW_KEY_RIGHT)) {
		// Move right
	} else if (GLFW_TEST_KEY(GLFW_KEY_UP)) {
		// Increase forward speed of camera
	} else if (GLFW_TEST_KEY(GLFW_KEY_DOWN)) {
		// Decrease forward speed of camera (no reverse!)
	} else if (GLFW_TEST_KEY(GLFW_KEY_SPACE)) {
		// Stop
	} else if (GLFW_TEST_KEY(GLFW_KEY_PAGEUP)) {
		// Increase elevation of camera
	} else if (GLFW_TEST_KEY(GLFW_KEY_PAGEDOWN)) {
		// Decrease elevation of camera
	} else if (TEST_KEY('R')) {
		// Reset all animation
	}
}
