//
//  KeyframeAnimator.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 05/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#include "KeyframeAnimator.h"

KeyframeAnimator::KeyframeAnimator(SceneObject *object) {
	originalTranslation = vec3(object->translation);
	originalRotation = vec3(object->rotation);
	this->object = object;
	currentKeyframeIndex = 0;
}

void KeyframeAnimator::reset() {
	currentKeyframeIndex = 0;
	timeOffset = glfwGetTime();
}

void KeyframeAnimator::animate() {
	
	if (keyframes.size() == 0) {
		return;
	}
	
	double currentTime = glfwGetTime() - timeOffset;

	Keyframe *from = keyframes[currentKeyframeIndex];
	Keyframe *to = keyframes[(currentKeyframeIndex + 1) % keyframes.size()];
	
	if (currentTime > to->time) {
		from = to;
		to = keyframes[(currentKeyframeIndex + 2) % keyframes.size()];
		currentKeyframeIndex = (currentKeyframeIndex + 1) % keyframes.size();
		if (from->time == 0) {
			timeOffset = glfwGetTime();
		}
	}
	
	Keyframe *now = interpolateKeyframes(from, to, currentTime);
	object->translation = originalTranslation + now->position;
	object->rotation = originalRotation + now->rotation;
}

Keyframe *KeyframeAnimator::interpolateKeyframes(Keyframe *k1, Keyframe *k2, double time) {
	Keyframe *now = new Keyframe();
	
	float lambda = (time - k1->time) / (k2->time - k1->time);
	
	now->position = k1->position + (lambda * (k2->position - k1->position));
	now->rotation = k1->rotation + (lambda * (k2->rotation - k1->rotation));
	
	return now;
}

void KeyframeAnimator::addKeyframe(Keyframe *keyframe) {
	keyframes.push_back(keyframe);
}

Keyframe *createKeyframe(double time, vec3 position, vec3 rotation) {
	Keyframe *k = new Keyframe;
	k->rotation = rotation;
	k->position = position;
	k->time = time;
	return k;
}
