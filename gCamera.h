#ifndef G_CAMERA_H__
#define G_CAMERA_H__

#include "gTools.h"
#include "gVec.h"
#include "gMat.h"

class gCamera {
public:
	virtual Vec3 getDir() = 0;
	virtual Vec3 getPos() = 0;
	virtual Vec3 getUp() = 0;

	Mat4 getLookAt() {
		Vec3 dir = getDir();
		Vec3 pos = getPos();
		Vec3 up = getUp();
		return Mat4::lookAt(pos, pos + dir, up);
	}
};

class gFocusCamera : public gCamera {
public:
	Vec3 focusPoint;
	float distanceToFocus;
	float angle;
	float elevation;

	gFocusCamera() {
		focusPoint = Vec3(0.0f, 0.0f, 0.0f);
		distanceToFocus = 1000.0f;
		angle = 0.0f;
		elevation = pi_d2*0.5f;
	}

	Vec3 getDir() override {
		return Vec3::normalize(Vec3(sin(angle)*cos(elevation), cos(angle)*cos(elevation), -sin(elevation)));
	}
	Vec3 getPos() override {
		return focusPoint - (getDir()*distanceToFocus);
	}
	Vec3 getUp() override {
		return Vec3(0.0f, 0.0f, 1.0f);
	}
};

#endif