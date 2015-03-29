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

	camera->focusPoint = Vec3(world->getMapSize() * WORLD_MAP_SCALE / 2, world->getMapSize() * WORLD_MAP_SCALE / 2, 0);
	camera->distanceToFocus = 100.0f;

	isFPS = false;

	fpsCamera = new gFPSCamera();
}

void Valhalla::tick(float dt) {
	if (input.isKeyDown(GLFW_KEY_V)) {
		glPolygonMode(GL_FRONT, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT, GL_FILL);
	}
}


void Valhalla::update(float fixed_dt) {

	if (input.isKeyPressed(GLFW_KEY_C)) {
		isFPS = !isFPS;
		if (isFPS) {
			world->frame.translateBy(Vec3(-camera->focusPoint.x, -camera->focusPoint.y, 0.0f));
			world->frame.scaleBy(1.0f / WORLD_MAP_SCALE);

			fpsCamera->pos = Vec3::zero();
			activeCamera = fpsCamera;
		} else {
			world->frame.makeIdentity();
			activeCamera = camera;
		}
	}


	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		world->build();
	}

	if (isFPS) {

		Vec3 dir = camera->getDir();
		dir.z = 0.0f;
		dir.normalize();
		Vec3 side = Vec3::cross(dir, Vec3(0.0f, 0.0f, 1.0f));

		if (input.isKeyDown(GLFW_KEY_W)) {
			fpsCamera->pos += dir * fixed_dt;
		} else if (input.isKeyDown(GLFW_KEY_S)) {
			fpsCamera->pos -= dir * fixed_dt;
		}

		if (input.isKeyDown(GLFW_KEY_A)) {
			fpsCamera->pos -= side * fixed_dt;
		} else if (input.isKeyDown(GLFW_KEY_D)) {
			fpsCamera->pos += side * fixed_dt;
		}

		fpsCamera->angle += input.getMouseDelta().x * 0.003f;
		fix_angle(camera->angle);
		fpsCamera->elevation += input.getMouseDelta().y * 0.003f;
		clamp(fpsCamera->elevation, -pi_d2*0.99f, pi_d2*0.99f);

		//float h = world->getHeightAt(camera->focusPoint.vec2 / WORLD_MAP_SCALE + fpsCamera->pos.vec2);

		//fpsCamera->pos.z = h + 2.0f;

	} else {
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

		//camera->focusPoint.z = world->getHeightAt(camera->focusPoint.vec2);

		if (input.isKeyDown(MOUSE_BUTTON_MID)) {
			camera->distanceToFocus *= 1.0f + (input.getMouseDelta().y * 0.003f);
		}

		if (input.isScrolled()) {
			camera->distanceToFocus *= 1.0f + (input.getScrollDelta().y * 0.03f);
		}
	}

}

void Valhalla::resize(int width, int height) {

}

