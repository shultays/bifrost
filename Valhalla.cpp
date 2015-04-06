#include "Valhalla.h"
#include <stdio.h>
#include "gInput.h"
#include "gGlobals.h"
#include "gTools.h"
#include "WorldMap.h"
#include "DetailedMapController.h"
#include "gSlave.h"

#include <functional>

void Valhalla::init() {
	activeCamera = camera = new gFocusCamera();

	world = new WorldMap();
	world->build();

	playerCoor.index = IntVec2(world->getEdgeCount() / 2, world->getEdgeCount() / 2);
	playerCoor.pos.setZero();

	isFPS = false;
	camera->distanceToFocus = 100.0f;

	fpsCamera = new gFPSCamera();

	detailedMapController = new DetailedMapController(world);
}

void Valhalla::tick(float dt) {
	if (input.isKeyDown(GLFW_KEY_Z)) {
		glPolygonMode(GL_FRONT, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT, GL_FILL);
	}
}

int t;
class SlaveTest : public gSlaveWork {
public:
	int id;
	SlaveTest() {
		id = t++;
	}
	virtual void runOnSlave() {
		int r = rand() % 5 + 4;
		printf("%d >> counting to %d\n", id, r);

		for (int i = 1; i <= r; i++) {
			printf("%d >> %d\n", id, i);
			sleepMS(1000);
		}
		printf("%d >> counting done\n", id);
	}
	virtual void runOnMain() {
		printf("%d >> work done\n", id);
	}
};



void Valhalla::update(float fixed_dt) {
	if (input.isKeyPressed(GLFW_KEY_J)) {
		gears.addSlaveWork(new SlaveTest());
	}
	if (input.isKeyPressed(GLFW_KEY_C)) {
		isFPS = !isFPS;
		world->setIsScaled(isFPS);
		if (isFPS) {
			world->setAnchorPos(playerCoor.index);

			world->frame.translateBy(Vec3((float)-playerCoor.index.x, (float)-playerCoor.index.y, 0.0f));
			world->frame.scaleBy(Vec3(world->getNodeSize(), world->getNodeSize(), world->getNodeSize() / 3.0f));

			Vec4 v(0.0f, 0.0f, 0.0f, 1.0f);
			v *= world->frame;

			activeCamera = fpsCamera;

			detailedMapController->initMap(playerCoor);
		} else {
			world->frame.makeIdentity();
			activeCamera = camera;
		}
	}


	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		world->build();
	}

	if (isFPS) {

		if (input.isKeyDown(MOUSE_BUTTON_LEFT)) {
			fpsCamera->angle += input.getMouseDelta().x * 0.003f;
			fix_angle(camera->angle);
			fpsCamera->elevation += input.getMouseDelta().y * 0.003f;
			clamp(fpsCamera->elevation, -pi_d2*0.99f, pi_d2*0.99f);
		}

		Vec2 dir = fpsCamera->getDir().vec2;
		dir.normalize();
		Vec2 side = dir * Mat2::rotation(pi_d2);

		if (input.isKeyDown(GLFW_KEY_W)) {
			playerCoor.pos += dir * fixed_dt * 20.0f;
		} else if (input.isKeyDown(GLFW_KEY_S)) {
			playerCoor.pos -= dir * fixed_dt * 20.0f;
		}

		if (input.isKeyDown(GLFW_KEY_A)) {
			playerCoor.pos -= side * fixed_dt * 20.0f;
		} else if (input.isKeyDown(GLFW_KEY_D)) {
			playerCoor.pos += side * fixed_dt * 20.0f;
		}

		playerCoor.fix(world->getNodeSize());


		Vec2 shift = (playerCoor.index - world->getAnchorPos()).toVec()*world->getNodeSize();

		fpsCamera->pos.vec2 = shift + playerCoor.pos;
		fpsCamera->pos.z = world->getHeightAt(playerCoor) + 1.8f;

		detailedMapController->updateMap(playerCoor);
	} else {
		if (input.isKeyDown(MOUSE_BUTTON_LEFT)) {
			camera->angle += input.getMouseDelta().x * 0.003f;
			fix_angle(camera->angle);
			camera->elevation += input.getMouseDelta().y * 0.003f;
			clamp(camera->elevation, 0, pi_d2*0.99f);
		}

		if (input.isKeyDown(MOUSE_BUTTON_MID)) {
			camera->distanceToFocus *= 1.0f + (input.getMouseDelta().y * 0.003f);
		}

		if (input.isScrolled()) {
			camera->distanceToFocus *= 1.0f + (input.getScrollDelta().y * 0.03f);
		}

		if (input.isKeyDown(MOUSE_BUTTON_RIGHT)) {
			Vec2 dir = camera->getDir().vec2;
			dir.normalize();
			Vec2 side = dir * Mat2::rotation(pi_d2);

			playerCoor.pos += dir * (input.getMouseDelta().y * camera->distanceToFocus * 10.0f);
			playerCoor.pos -= side * (input.getMouseDelta().x * camera->distanceToFocus * 10.0f);
		}


		playerCoor.fix(world->getNodeSize());



		camera->focusPoint = Vec3(playerCoor.index.x + playerCoor.pos.x / world->getNodeSize(), playerCoor.index.y + playerCoor.pos.y / world->getNodeSize(), 0.0f);

	}

}

void Valhalla::resize(int width, int height) {

}

