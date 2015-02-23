#include "Valhalla.h"
#include <stdio.h>
#include "gInput.h"
#include "gGlobals.h"
#include "gTools.h"
#include "WorldMap.h"

#include <functional>

void Valhalla::init() {
	gFocusCamera* camera;
	activeCamera = camera = new gFocusCamera();
	camera->focusPoint = Vec3(0.0f, 0.0f, 0.0f);
	camera->distanceToFocus = 1000.0f;
	camera->angle = 0.0f;

	world = new WorldMap();
	world->build();
}

void Valhalla::tick(float dt) {

	((gFocusCamera*)activeCamera)->angle += dt*0.3f;
}


void Valhalla::update(float fixed_dt) {
	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		world->build();

	}

}

void Valhalla::resize(int width, int height) {

}

