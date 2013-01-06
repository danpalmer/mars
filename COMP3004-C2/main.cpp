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
#include "KeyframeAnimator.h"
#include "CameraKeyframeAnimator.h"

using namespace glm;
using namespace std;

void initGraphics();
void render(int frame, vector<SceneObject *> sceneObjects, vector<glm::vec3> sceneLights, GLuint shader);
void GLFWCALL keyHandler(int key, int action);
void checkKeyHolds();
void initSceneObjects();

bool onTour = false;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

Camera *camera = new Camera();
CameraKeyframeAnimator *cameraAnimator = new CameraKeyframeAnimator(camera);
vector<SceneObject *> sceneObjects;
vector<KeyframeAnimator *> animators;

int main(int argc, const char * argv[]) {
	int running = GL_TRUE;
	initGraphics();
	
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
	
	glfwSetMousePos(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	
	int i = 0;
	static double t0 = glfwGetTime();
	static double fps = 60.0;
	while (running) {
		if ((glfwGetTime() - t0) > (1.0/fps)) {
			t0 = glfwGetTime();
			i++;
		}
		
		camera->computeMatricesFromInputs(onTour);
		render(i, sceneObjects, sceneLights, shader);
		
		glFlush();
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
	
    return 0;
}

void initGraphics() {
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
	
	if (!glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 0, 0, 0, 1, GLFW_WINDOW)) {
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
	
	for (int i = 0; i < animators.size(); i++) {
		animators[i]->animate();
	}
	
	if (onTour) {
		bool tourComplete = cameraAnimator->animate();
		if (tourComplete) {
			onTour = false;
			camera->resetToPosition0();
		}
	}
	
	mat4 perspective = camera->getProjectionMatrix();
	mat4 view = camera->getViewMatrix();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(glGetUniformLocation(shader, "perspective"), 1, GL_FALSE, value_ptr(perspective));
	glUniform1i(glGetUniformLocation(shader, "lightcount"), (GLint)sceneLights.size());
	glUniform3fv(glGetUniformLocation(shader, "lightpos"), 1, value_ptr(sceneLights[0]));
	vec3 camerapos = camera->getPosition();
	glUniform3fv(glGetUniformLocation(shader, "camerapos"), 1, value_ptr(camerapos));
	
	for (int i = 0; i < sceneObjects.size(); i++) {
		SceneObject *obj = sceneObjects[i];
		glUniform1i(glGetUniformLocation(shader, "surface"), (GLint)sceneObjects[i]->surfaceType);
		
		view = translate(view, -obj->translation);
		mat4 model = mat4(1.0);
		model = rotate(model, obj->rotation.x, vec3(1.0, 0.0, 0.0));
		model = rotate(model, obj->rotation.y, vec3(0.0, 1.0, 0.0));
		model = rotate(model, obj->rotation.z, vec3(0.0, 0.0, 1.0));
		
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, value_ptr(view));
		sceneObjects[i]->render(shader);
		
		view = translate(view, obj->translation);
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
		camera->resetToPosition0();
	} else if (TEST_KEY('Y')) {
		// Move to alternative location 1
		camera->resetToPosition1();
	} else if (TEST_KEY('U')) {
		// Move to alternative location 2
		camera->resetToPosition2();
	} else if (TEST_KEY('T')) {
		// Start tour, ignore other key presses except exit
		onTour = true;
		cameraAnimator->reset();
		for (int i = 0; i < animators.size(); i++) {
			animators[i]->reset();
		}
	} else if (TEST_KEY('R')) {
		// Reset all animation
		for (int i = 0; i < animators.size(); i++) {
			animators[i]->reset();
		}
	} else if (TEST_KEY('C')) {
		fprintf(stdout, "cameraAnimator->addKeyframe(createCameraKeyframe(TIME, vec3(%f, %f, %f), %f, %f, 45));\n",
				camera->getPosition().x,
				camera->getPosition().y,
				camera->getPosition().z,
				camera->horizontalAngle,
				camera->verticalAngle);
	}
}
#undef TEST_KEY

void initSceneObjects() {
	
	// Static objects
	
	/*
	 Main building bases
	 */
	SceneObject *bases = new SceneObject("bases.obj");
	bases->setMaterial(vec4(0.407946, 0.407946, 0.407946, 1.000000), 0.1, 0.9, 0.4, 1000.0);
	bases->buffer();
	sceneObjects.push_back(bases);

	
	/*
	 Building foundations
	 */
	SceneObject *foundation = new SceneObject("foundation.obj");
	foundation->setMaterial(vec4(0.223571, 0.223571, 0.223571, 1.000000), 0.1, 0.9, 0.4, 1000.0);
	foundation->buffer();
	sceneObjects.push_back(foundation);
	
	
	/*
	 Bridge
	 */
	SceneObject *bridge = new SceneObject("bridge.obj");
	bridge->setMaterial(vec4(0.453661, 0.332416, 0.332416, 1.000000), 0.2, 0.5, 0.3, 200.0);
	bridge->buffer();
	sceneObjects.push_back(bridge);
	
	
	/* 
	 Buildings
	 */
	SceneObject *buildings = new SceneObject("buildings.obj");
	buildings->setMaterial(vec4(0.501949, 0.614796, 0.614796, 1.000000), 0.2, 0.8, 0.6, 1000.0);
	buildings->buffer();
	sceneObjects.push_back(buildings);

	
	/*
	 Skybox
	 */
	Skybox *skybox = new Skybox("mars");
	skybox->buffer();
	sceneObjects.push_back(skybox);
	
	
	// Animated Objects
#define KEYFRAME(animator, time, position, rotation) animator->addKeyframe(createKeyframe(time, position, rotation));
	
	/*
	 Bridge Car 1
	 */
	SceneObject *car1 = new SceneObject("car.obj");
	car1->setMaterial(BLUE, 0.1, 0.1, 0.8, 3000);
	car1->translation = vec3(-137, -391, 1300);
	car1->buffer();
	sceneObjects.push_back(car1);
	
	KeyframeAnimator *car1Animator = new KeyframeAnimator(car1);
	KEYFRAME(car1Animator, 0, vec3(0), vec3(0))
	KEYFRAME(car1Animator, 10, vec3(0, 0, 1250), vec3(0))
	KEYFRAME(car1Animator, 12, vec3(-30, 0, 1250), vec3(0, -180, 0))
	KEYFRAME(car1Animator, 22, vec3(-30, 0, 0), vec3(0, -180, 0))
	KEYFRAME(car1Animator, 24, vec3(0,0,0), vec3(0,0,0))
	animators.push_back(car1Animator);
	

	/*
	 Bridge Car 2
	 */
	SceneObject *car2 = new SceneObject("car.obj");
	car2->setMaterial(RED, 0.1, 0.1, 0.8, 3000);
	car2->translation = vec3(-137, -391, 1300);
	car2->buffer();
	sceneObjects.push_back(car2);
	
	KeyframeAnimator *car2Animator = new KeyframeAnimator(car2);
	KEYFRAME(car2Animator, 0, vec3(-30, 0, 1250), vec3(0))
	KEYFRAME(car2Animator, 10, vec3(-30, 0, 0), vec3(0))
	KEYFRAME(car2Animator, 12, vec3(0, 0, 0), vec3(0, -180, 0))
	KEYFRAME(car2Animator, 22, vec3(0, 0, 1250), vec3(0, -180, 0))
	KEYFRAME(car2Animator, 24, vec3(-30,0,1250), vec3(0,0,0))
	animators.push_back(car2Animator);
	
	
	/*
	 Ship 1 (taking off)
	 */
	SceneObject *ship1 = new SceneObject("ship.obj");
	ship1->setMaterial(vec4(0.561966, 0.782683, 0.782683, 1.000000), 0.1, 0.9, 0.7, 3000);
	ship1->translation = vec3(-364, -236, 912);
	ship1->buffer();
	sceneObjects.push_back(ship1);
	
	KeyframeAnimator *ship1Animator = new KeyframeAnimator(ship1);
	KEYFRAME(ship1Animator, 0, vec3(0), vec3(0))
	KEYFRAME(ship1Animator, 58, vec3(0), vec3(0))
	KEYFRAME(ship1Animator, 59.5, vec3(0, -20, -20), vec3(20, 0, 0))
	KEYFRAME(ship1Animator, 63, vec3(-300, -40, -320), vec3(10, 30, -30))
	KEYFRAME(ship1Animator, 71, vec3(-12000, -300, -12000), vec3(0, 720, 0)) // do a barrel roll
	animators.push_back(ship1Animator);
	
	
	/*
	 Ship 2 (figure 8)
	 */
	SceneObject *ship2 = new SceneObject("ship.obj");
	ship2->setMaterial(vec4(0.561966, 0.782683, 0.782683, 1.000000), 0.1, 0.9, 0.7, 3000);
	ship2->translation = vec3(-800, -1000, 800);
	ship2->buffer();
	sceneObjects.push_back(ship2);
	
#define TURN_RADIUS 500
	KeyframeAnimator *ship2Animator = new KeyframeAnimator(ship2);
	KEYFRAME(ship2Animator,  0, vec3(0, 0, 0),						vec3(0, 0, 0))
	KEYFRAME(ship2Animator,  2, vec3(TURN_RADIUS, 0, -TURN_RADIUS),	vec3(0, -90, 30))
	KEYFRAME(ship2Animator,  4, vec3(2* TURN_RADIUS, 0, 0),			vec3(0, -180, 30))
	KEYFRAME(ship2Animator,  6, vec3(TURN_RADIUS, 0, TURN_RADIUS), vec3(0, -270, 30))
	
	KEYFRAME(ship2Animator,  8, vec3(0, 0, 0),						vec3(0, -360, 0))
	KEYFRAME(ship2Animator, 10, vec3(-TURN_RADIUS, 0, -TURN_RADIUS), vec3(0, -270, -30))
	KEYFRAME(ship2Animator, 12, vec3(-2* TURN_RADIUS, 0, 0),		vec3(0, -180, -30))
	KEYFRAME(ship2Animator, 14, vec3(-TURN_RADIUS, 0, TURN_RADIUS),vec3(0, -90, -30))
	
	KEYFRAME(ship2Animator, 16, vec3(0, 0, 0),						vec3(0, 0, 0))
#undef TURN_RADIUS
	animators.push_back(ship2Animator);
	
#undef KEYFRAME
	
	
	// Tour Animation
#define SHORT (time += 1)
#define MEDIUM (time += 3)
#define LONG (time += 6)
#define WIDE_FOV 75
#define NORMAL_FOV 45
#define ZOOM_FOV 20
	float time = 0;
	cameraAnimator->addKeyframe(createCameraKeyframe(time, vec3(150.000000, 1000.000000, -5000.000000), 0.000000, -0.250000, NORMAL_FOV));		// start
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(998.909302, 785.915039, -4656.653809), -0.667321, -0.047858, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(-672.809692, 657.641174, -4148.628906), 0.502348, -0.173394, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(216.018677, 391.614532, -3286.671143), 0.108119, -0.015488, NORMAL_FOV));		// end opening
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(237.933456, 397.782043, -3086.380127), -0.573812, 0.253406, WIDE_FOV));		// start gatehouse
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(340.526001, 395.940704, -3004.105713), -0.593840, 0.127725, WIDE_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(319.041504, 407.835754, -2710.398682), -2.210917, 0.103304, WIDE_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(SHORT, vec3(195.028229, 408.228882, -2663.615723), -0.499117, -0.031121, WIDE_FOV));		// end gatehouse
	cameraAnimator->addKeyframe(createCameraKeyframe(SHORT, vec3(162.627991, 406.113037, -2603.870361), -0.156223, -0.065116, NORMAL_FOV));		// start bridge
	cameraAnimator->addKeyframe(createCameraKeyframe(LONG, vec3(161.485489, 397.687958, -2301.598145), 0.011810, 0.010723, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(165.569214, 406.901978, -1684.026367), 0.003080, -0.031551, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(168.063965, 416.927124, -1253.849365), 0.058967, -0.043376, NORMAL_FOV));		// end bridge
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(226.445984, 455.948242, -1044.670288), -0.243098, 1.094442, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(148.719391, 1250.437988, -904.740540), -0.024578, 1.275791, NORMAL_FOV));		// end tower
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(146.938766, 2483.820557, -988.341248), -0.019644, -1.616784, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(MEDIUM, vec3(-1134.093384, 606.541382, -1548.947998), 1.191051, -0.173938, ZOOM_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(LONG, vec3(25.547859, 882.380249, 642.614746), 3.113941, -0.259662, NORMAL_FOV));
	cameraAnimator->addKeyframe(createCameraKeyframe(LONG, vec3(1787.619507, 142.188950, -1110.956055), 4.883299, 0.094412, ZOOM_FOV));			// ship take off at 50s
	cameraAnimator->addKeyframe(createCameraKeyframe(LONG, vec3(1787.619507, 142.188950, -1110.956055), 4.883299, 0.094412, NORMAL_FOV));		// watch ship take off
	cameraAnimator->addKeyframe(createCameraKeyframe(LONG, vec3(3606.669434, -32.609947, -1424.913330), 4.883299, 0.094412, NORMAL_FOV));
#undef WIDE_FOV
#undef NORMAL_FOV
#undef ZOOM_FOV
#undef LONG
#undef MEDIUM
#undef LONG
	
	// Load the landscape last because that's the largest part and we want to show progress in the console.
	
	/*
	 Landscape
	 */
	SceneObject *landscape = new SceneObject("mars-landscape.obj");
	landscape->texture = new Texture("ground_texture.tga", 0, true);
	landscape->surfaceType = texturedAndLit;
	landscape->setMaterial(vec4(0.5922, 0.3216, 0.2588, 1.0), 0.0, 1.0, 0.0, 0.0);
	landscape->buffer();
	sceneObjects.push_back(landscape);
}
