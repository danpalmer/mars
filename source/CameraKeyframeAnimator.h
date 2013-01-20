//
//  CameraKeyframeAnimator.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 05/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__CameraKeyframeAnimator__
#define __COMP3004_C2__CameraKeyframeAnimator__

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

using namespace glm;
using namespace std;

typedef struct {
	double time;
	vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float fieldOfView;
} CameraKeyframe;

CameraKeyframe *createCameraKeyframe(double time, vec3 position, float horizontalAngle, float verticalAngle, float fieldOfView);

class CameraKeyframeAnimator {
	vector<CameraKeyframe *> keyframes;
	int currentKeyframeIndex;
	Camera *camera;
	double timeOffset;
	
public:
	CameraKeyframeAnimator(Camera *camera);
	bool animate();
	CameraKeyframe *interpolateKeyframes(CameraKeyframe *k1, CameraKeyframe *k2, double time);
	void addKeyframe(CameraKeyframe *keyframe);
	void reset();
};

#endif /* defined(__COMP3004_C2__CameraKeyframeAnimator__) */
