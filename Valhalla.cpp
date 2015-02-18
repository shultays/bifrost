#include "Valhalla.h"
#include <stdio.h>
#include "gInput.h"
#include "gGlobals.h"
#include "gTools.h"

void Valhalla::init(){
	gFocusCamera* camera;
	activeCamera = camera  = new gFocusCamera();
	camera->focusPoint = Vec3(0.0f, 0.0f, 0.0f);
	camera->distanceToFocus = 40.0f;
	camera->angle = 0.0f;
}

void Valhalla::tick(float dt){
	Mat4 test = Mat4::rotation(0.3f, Vec3(1.0f, 0.0f, 0.0f));
	Mat4 test2 = Mat4::rotationX(0.3f);
	bool t = test == test2;

	test = Mat4::rotation(0.3f, Vec3(0.0f, 1.0f, 0.0f));
	test2 = Mat4::rotationY(0.3f);

	bool t2 = test == test2;

	test = Mat4::rotation(0.3f, Vec3(0.0f, 0.0f, 1.0f));
	test2 = Mat4::rotationZ(0.3f);

	bool t3 = test == test2;

	int bcasd = 5;
}



void Valhalla::resize(int width, int height){

}

