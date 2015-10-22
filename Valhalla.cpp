#include "Valhalla.h"
#include <stdio.h>
#include "gInput.h"
#include "gGlobals.h"
#include "gTools.h"
#include "WorldMap.h"
#include "DetailedMapGenerator.h"
#include "gCamera.h"

#include "gModelReader.h"
#include "gRenderableGroup.h"
#include "TreeGenerator.h"
#include "ForestGenerator.h"
#include "Sky.h"
#include "gBinaryStream.h"


void Valhalla::init() {
	activeCamera = camera = new gFocusCamera();

	world = new WorldMap(256 * 1024.0f);
	if (true) {
		world->build();
		gBinaryFileOutputStream bOutput;
		bOutput.open("data.dat", std::ifstream::in | std::ifstream::binary);
		world->serialize(bOutput);
		bOutput.close();
	} else {

		gBinaryFileInputStream bInput;
		bInput.open("data.dat", std::ifstream::in | std::ifstream::binary);
		world->deserialize(bInput);

		bInput.close();
		world->buildBuffer();

	}


	playerCoor.index = IntVec2(world->getEdgeCount() / 2, world->getEdgeCount() / 2);
	playerCoor.pos.setZero();

	isFPS = false;
	camera->distanceToFocus = 100.0f;

	fpsCamera = new gFPSCamera();

	detailedMapController = new DetailedMapGenerator(world, 7, 256, 8);
	detailedMapController2 = new DetailedMapGenerator(world, 3, 1, 16);
	forestGenerator = new ForestGenerator(world);
	sky = new Sky(world);


	gShaderShr worldShader = resources.getShader("default.vs", "terrain.ps");
	worldShader->addUniform("uDiscardArea", TypeVec4);

	world->shader = worldShader;
}

void Valhalla::tick(float dt) {
	if (input.isKeyDown(GLFW_KEY_Z)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	detailedMapController2->setCropArea(detailedMapController->getTerrainArea());
}


void Valhalla::update(float fixed_dt) {
	if (input.isKeyPressed(GLFW_KEY_C)) {
		bool hasJob = detailedMapController->hasWaitingJob() || detailedMapController2->hasWaitingJob() || forestGenerator->hasWaitingJob();
		if (hasJob == false) {
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
				detailedMapController2->initMap(playerCoor);
				forestGenerator->initMap(playerCoor);
				//sky->initSky(playerCoor);

			} else {
				world->frame.makeIdentity();
				activeCamera = camera;

				detailedMapController->deleteMap();
				detailedMapController2->deleteMap();
				forestGenerator->deleteMap();
				sky->deleteSky();
			}
		}
	}


	if (input.isKeyPressed(GLFW_KEY_SPACE)) {
		debugRenderer.clear();
		world->buildDrainage();
		world->buildRivers();
	}

	if (isFPS) {
		if (input.isKeyPressed(GLFW_KEY_2)) {
			WorldCoor treeWorldCoor = playerCoor;

			debugRenderer.addSphere(world->toGamePos(treeWorldCoor) + fpsCamera->getDir()*2.0f + Vec3(0.0f, 0.0f, 2.0f), 0.5f, 0xFFFFFFFF, 5.0f);
		}

		float speed = 2.0f;
		if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) speed = 20.0f;

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
			playerCoor.pos += dir * fixed_dt * speed;
		} else if (input.isKeyDown(GLFW_KEY_S)) {
			playerCoor.pos -= dir * fixed_dt * speed;
		}

		if (input.isKeyDown(GLFW_KEY_A)) {
			playerCoor.pos -= side * fixed_dt * speed;
		} else if (input.isKeyDown(GLFW_KEY_D)) {
			playerCoor.pos += side * fixed_dt * speed;
		}

		playerCoor.fix(world->getNodeSize());


		Vec2 shift = (playerCoor.index - world->getAnchorPos()).toVec()*world->getNodeSize();
		Vec3 oldPos = fpsCamera->pos;

		fpsCamera->pos.vec2 = shift + playerCoor.pos;
		fpsCamera->pos.z = world->getHeightAt(playerCoor) + 1.8f * 1.0f;

		if (input.isKeyDown(GLFW_KEY_R)) {
			debugRenderer.addLine(oldPos, fpsCamera->pos, 0xFF00FFFF, 1.0f);
		}

		detailedMapController->updateMap(playerCoor);
		detailedMapController2->updateMap(playerCoor);
		forestGenerator->updateMap(playerCoor);
		sky->updateSky(playerCoor);
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

