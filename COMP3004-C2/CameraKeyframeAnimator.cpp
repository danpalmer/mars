//
//  CameraKeyframeAnimator.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 05/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#include "CameraKeyframeAnimator.h"

CameraKeyframeAnimator::CameraKeyframeAnimator(Camera *camera) {
	this->camera = camera;
	currentKeyframeIndex = 0;
}

void CameraKeyframeAnimator::animate() {
	
	if (keyframes.size() == 0) {
		return;
	}
	
	double currentTime = glfwGetTime() - timeOffset;
	
	CameraKeyframe *from = keyframes[currentKeyframeIndex];
	CameraKeyframe *to = keyframes[(currentKeyframeIndex + 1) % keyframes.size()];
	
	if (currentTime > to->time) {
		from = to;
		to = keyframes[(currentKeyframeIndex + 2) % keyframes.size()];
		currentKeyframeIndex = (currentKeyframeIndex + 1) % keyframes.size();
		if (from->time == 0) {
			timeOffset = glfwGetTime();
		}
	}
	
	CameraKeyframe *now = interpolateKeyframes(from, to, currentTime);
	camera->position = now->position;
	camera->horizontalAngle = now->horizontalAngle;
	camera->verticalAngle = now->verticalAngle;
	camera->fieldOfView = now->fieldOfView;
}

CameraKeyframe *CameraKeyframeAnimator::interpolateKeyframes(CameraKeyframe *k1, CameraKeyframe *k2, double time) {
	CameraKeyframe *now = new CameraKeyframe();
	
	float lambda = (time - k1->time) / (k2->time - k1->time);
	
	now->position = k1->position + (lambda * (k2->position - k1->position));
	now->fieldOfView = k1->fieldOfView + (lambda * (k2->fieldOfView - k1->fieldOfView));
	now->verticalAngle = k1->verticalAngle + (lambda * (k2->verticalAngle - k1->verticalAngle));
	now->horizontalAngle = k1->horizontalAngle + (lambda * (k2->horizontalAngle - k1->horizontalAngle));
	
	return now;
}

void CameraKeyframeAnimator::addKeyframe(CameraKeyframe *keyframe) {
	keyframes.push_back(keyframe);
}

CameraKeyframe *createCameraKeyframe(double time, vec3 position, float horizontalAngle, float verticalAngle, float fieldOfView) {
	CameraKeyframe *k = new CameraKeyframe;
	k->position = position;
	k->fieldOfView = fieldOfView;
	k->horizontalAngle = horizontalAngle;
	k->verticalAngle = verticalAngle;
	k->time = time;
	return k;
}
