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

	WorldMap *world = new WorldMap(2048, 128);
	world->build();
}

void Valhalla::tick(float dt) {

	((gFocusCamera*)activeCamera)->angle += dt*0.3f;
}



void Valhalla::resize(int width, int height) {

}

