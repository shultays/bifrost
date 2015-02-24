#include "Valhalla.h"
#include <stdio.h>
#include "gInput.h"
#include "gGlobals.h"
#include "gTools.h"
#include "WorldMap.h"

#include <functional>

void Valhalla::init() {
	activeCamera = camera = new gFocusCamera();

	world = new WorldMap();
	world->build();
}

void Valhalla::tick(float dt) {


}


void Valhalla::update(float fixed_dt) {
	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		world->build();

	}

	if (input.isKeyDown(MOUSE_BUTTON_LEFT)) {
		camera->angle += input.getMouseDelta().x * 0.003f;
		fix_angle(camera->angle);
		camera->elevation += input.getMouseDelta().y * 0.003f;
		clamp(camera->elevation, 0, pi_d2*0.99f);
	}

	if (input.isKeyDown(MOUSE_BUTTON_RIGHT)) {
		Vec3 dir = camera->getDir();
		dir.z = 0.0f;
		dir.normalize();
		Vec3 side = dir.cross(camera->getUp());
		camera->focusPoint += dir * (input.getMouseDelta().y * camera->distanceToFocus * 0.003f);
		camera->focusPoint -= side * (input.getMouseDelta().x * camera->distanceToFocus * 0.003f);
	}

	if (input.isKeyDown(MOUSE_BUTTON_MID)) {
		camera->distanceToFocus *= 1.0f + (input.getMouseDelta().y * 0.003f);
	}

	if (input.isScrolled()) {
		camera->distanceToFocus *= 1.0f + (input.getScrollDelta().y * 0.03f);
	}
}

void Valhalla::resize(int width, int height) {

}

