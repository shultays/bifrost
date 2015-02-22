#include "gGame.h"
#include "gGlobals.h"
#include <stdio.h>
#include <stdlib.h>

void gGame::render() {
	Mat4 projection = Mat4::perspective(100.0f, 2500.0f, degreeToRadian(90.0f), (float)gears.width, (float)gears.height);
	Mat4 world = activeCamera->getLookAt();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection.data);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(world.data);

	Vec3 pos = activeCamera->getPos();
	Vec3 focus = pos + activeCamera->getDir();
	Vec3 up = activeCamera->getUp();


	shader.begin();
	shader.setWorldViewMatrix(world);
	shader.setProjectionMatrix(projection);
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
	for (int i = renderList.size() - 1; i >= 0; i--) {
		if (renderList[i]->enabled) renderList[i]->render();
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

	glBegin(GL_QUADS);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(+10.0f, -10.f, 0.0f);
	glVertex3f(+10.0f, +10.f, 0.0f);
	glVertex3f(-10.0f, +10.f, 0.0f);
	glVertex3f(-10.0f, -10.f, 0.0f);


	glEnd();
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
	if (autoAdd) {
		gears.game->addRenderable(this, priority);
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
	shader.loadFromFile("default.vs", "default.ps");
}
