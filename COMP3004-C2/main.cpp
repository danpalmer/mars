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

#include "controls.h"
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
void initSceneObjects();

bool onTour = false;

Camera *camera = new Camera();
vector<SceneObject *> sceneObjects;

int main(int argc, const char * argv[]) {
	int running = GL_TRUE;
	init();
	
	vector<GLint> shaders;
	shaders.push_back(setupShader("VertexShader.vert", GL_VERTEX_SHADER));
	shaders.push_back(setupShader("FragmentShader.frag", GL_FRAGMENT_SHADER));
	
	GLuint shader = linkShaders(shaders);
	glUseProgram(shader);
	check("Setup Shaders");
	
	initSceneObjects();
	vector<vec3> sceneLights;
	vec3 sun = vec3(-5000.0, 5000.0, 5000.0);
	sceneLights.push_back(sun);
	
	int i = 0;
	static double t0 = glfwGetTime();
	static double fps = 60.0;
	while (running) {
		if ((glfwGetTime() - t0) > (1.0/fps)) {
			t0 = glfwGetTime();
			i++;
		}
		
		camera->computeMatricesFromInputs();
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
	
	glfwDisable(GLFW_MOUSE_CURSOR);
	
	glfwSetKeyCallback(keyHandler);
}

void render(int frame, vector<SceneObject *> sceneObjects, vector<vec3> sceneLights, GLuint shader) {
	
	mat4 perspective = camera->getProjectionMatrix();
	mat4 model = mat4(1.0f);
	mat4 view = camera->getViewMatrix();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "perspective"), 1, GL_FALSE, value_ptr(perspective));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(view));
	
	glUniform1i(glGetUniformLocation(shader, "lightcount"), (GLint)sceneLights.size());
	glUniform3fv(glGetUniformLocation(shader, "lightpos"), 1, value_ptr(sceneLights[0]));
	vec3 camerapos = camera->getPosition();
	glUniform3fv(glGetUniformLocation(shader, "camerapos"), 1, value_ptr(camerapos));
	
	for (int i = 0; i < sceneObjects.size(); i++) {
		glUniform1i(glGetUniformLocation(shader, "surface"), (GLint)sceneObjects[i]->surfaceType);
		sceneObjects[i]->render();
	}
}

#define TEST_KEY(k) (key == k || key == (k + 32))
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
#undef TEST_KEY

void initSceneObjects() {
	
	SceneObject *landscape = new SceneObject("mars-landscape.obj");
	landscape->texture = new Texture("ground_texture.tga", 0, true);
	landscape->surfaceType = texturedAndLit;
	landscape->setMaterial(vec4(0.5922, 0.3216, 0.2588, 1.0), 0.0, 1.0, 0.0, 0.0);
	landscape->buffer();
	sceneObjects.push_back(landscape);
	
	SceneObject *bases = new SceneObject("bases.obj");
	bases->setMaterial(vec4(0.407946, 0.407946, 0.407946, 1.000000), 0.1, 0.9, 0.4, 1000.0);
	bases->buffer();
	sceneObjects.push_back(bases);

	SceneObject *foundation = new SceneObject("foundation.obj");
	foundation->setMaterial(vec4(0.223571, 0.223571, 0.223571, 1.000000), 0.1, 0.9, 0.4, 1000.0);
	foundation->buffer();
	sceneObjects.push_back(foundation);
	
	SceneObject *bridge = new SceneObject("bridge.obj");
	bridge->setMaterial(vec4(0.453661, 0.332416, 0.332416, 1.000000), 0.2, 0.8, 0.2, 200.0);
	bridge->buffer();
	sceneObjects.push_back(bridge);
	
	SceneObject *buildings = new SceneObject("buildings.obj");
	buildings->setMaterial(vec4(0.501949, 0.614796, 0.614796, 1.000000), 0.5, 0.5, 0.2, 1000.0);
	buildings->buffer();
	sceneObjects.push_back(buildings);
	
	Skybox *skybox = new Skybox("mars");
	skybox->buffer();
	sceneObjects.push_back(skybox);
}
