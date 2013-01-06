//
//  KeyframeAnimator.h
//  COMP3004-C2
//
//  Created by Dan Palmer on 05/01/2013.
//  Copyright (c) 2013 Dan Palmer. All rights reserved.
//

#ifndef __COMP3004_C2__KeyframeAnimator__
#define __COMP3004_C2__KeyframeAnimator__

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneObject.h"

using namespace glm;
using namespace std;

typedef struct {
	double time;
	vec3 position;
	vec3 rotation;
} Keyframe;

Keyframe *createKeyframe(double time, vec3 position, vec3 rotation);

class KeyframeAnimator {
	vector<Keyframe *> keyframes;
	int currentKeyframeIndex;
	SceneObject *object;
	vec3 originalTranslation;
	vec3 originalRotation;
	double timeOffset;
	
public:
	KeyframeAnimator(SceneObject *object);
	void animate();
	Keyframe *interpolateKeyframes(Keyframe *k1, Keyframe *k2, double time);
	void addKeyframe(Keyframe *keyframe);
	void reset();
};

#endif /* defined(__COMP3004_C2__KeyframeAnimator__) */
