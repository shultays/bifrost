#ifndef G_CAMERA_H__
#define G_CAMERA_H__

#include "gTools.h"

class gCamera{
	virtual vec3 getDir() = 0;
	virtual vec3 getPos() = 0;
	virtual vec3 getUp() = 0;

	void doLookAt(){
		vec3 dir = getDir();
		vec3 pos = getDir();
		vec3 up = getDir();
		gluLookAt(vec3_to_3_float((pos-dir)), vec3_to_3_float(pos), vec3_to_3_float(up));
	}
};

class gFocusCamera : public gCamera{
	vec3 focusPoint;
	float distanceToFocus;
	float angle;

	vec3 getDir() override{
		return normalize(vec3(sin(angle), cos(angle), -0.5f));
	}
	vec3 getPos() override{
		return focusPoint + getDir()*distanceToFocus;
	}
	vec3 getUp() override{
		return vec3(0.0f, 0.0f, 1.0f);
	}
};

#endif