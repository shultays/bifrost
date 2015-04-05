#include "gGame.h"
#include "gGlobals.h"
#include <stdio.h>
#include <stdlib.h>

void gGame::render() {
	updateProjectionMatrix();
	updateViewMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vec3 pos = activeCamera->getPos();
	Vec3 focus = pos + activeCamera->getDir();
	Vec3 up = activeCamera->getUp();

	resetWorldStack();
	shader.begin();
	shader.setColor(Vec4(1.0, 1.0f, 1.0f, 1.0f));
	/*
	//
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	glLoadMatrixf(world.data);
	//




	glShadeModel(GL_SMOOTH);*/
	for (int i = 0; i < renderList.size(); i++) {
		if (renderList[i]->enabled) renderList[i]->gRender();
	}
	/*
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	glLoadMatrixf(world.data);


	glDisable(GL_LIGHTING);
	*/
	/*
	glBegin(GL_QUADS);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(+10.0f, -10.f, 0.0f);
	glVertex3f(+10.0f, +10.f, 0.0f);
	glVertex3f(-10.0f, +10.f, 0.0f);
	glVertex3f(-10.0f, -10.f, 0.0f);

	glEnd();*/
}

void gGame::updateProjectionMatrix() {
	Mat4 projection = Mat4::perspective(activeCamera->nearPlane, activeCamera->farPlane, degreeToRadian(90.0f), (float)gears.width, (float)gears.height);
	shader.setProjectionMatrix(projection);
}
void gGame::updateViewMatrix() {
	Mat4 view = activeCamera->getLookAt();
	shader.setViewMatrix(view);
}

gTickable::gTickable(bool autoAdd) {
	enabled = true;
	if (autoAdd) {
		gears.game->addTickable(this);
	}
}

gTickable::~gTickable() {
	enabled = true;
	gears.game->removeTickable(this);
}

gUpdatable::gUpdatable(bool autoAdd, int priority) {
	enabled = true;
	if (autoAdd) {
		gears.game->addUpdatable(this, priority);
	}
}

gUpdatable::~gUpdatable() {
	enabled = true;
	gears.game->removeUpdatable(this);
}

gRenderable::gRenderable(bool autoAdd, int priority) {
	enabled = true;
	frame.makeIdentity();
	if (autoAdd) {
		gears.game->addRenderable(this, priority);
	}
}

void gRenderable::gRender() {
	bool identity = frame.isIdentity();
	if (!identity) {
		gears.game->pushMatrix();
		gears.game->multiply(frame);
	}
	gears.game->updateShaderUniforms();
	render();
	if (!identity) {
		gears.game->popMatrix();
	}
}

gRenderable::~gRenderable() {
	enabled = true;
	gears.game->removeRenderable(this);
}

gGame::gGame(bool setTop) {
	if (setTop) {
		gears.game = this;
	}
	currentStack = 0;
	shader.loadFromFile("default.vs", "default.ps");
}
