#include "gGame.h"
#include "gGlobals.h"
#include "gShader.h"
#include "gCamera.h"

#include <stdio.h>
#include <stdlib.h>

void gGame::render() {
	resetWorldStack();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentShader = nullptr;

	for (int i = 0; i < renderList.size(); i++) {
		if (renderList[i]->enabled) renderList[i]->gRender();
	}

}

void gGame::updateProjectionMatrix() {
	Mat4 projection = Mat4::perspective(activeCamera->nearPlane, activeCamera->farPlane, degreeToRadian(90.0f), (float)gears.width, (float)gears.height);
	currentShader->setProjectionMatrix(projection);
}
void gGame::updateViewMatrix() {
	Mat4 view = activeCamera->getLookAt();
	currentShader->setViewMatrix(view);
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
	shader = gears.defaultShader;
	if (autoAdd) {
		gears.game->addRenderable(this, priority);
	}
}

void gRenderable::gRender() {
	if (!enabled) return;
	if (shader.getObject() != gears.game->currentShader) {
		gears.game->setShader(shader.getObject());
	}
	bool identity = frame.isIdentity() && false;
	if (!identity) {
		gears.game->pushMatrix();
		gears.game->multiply(frame);
	}
	gears.game->updateShaderUniforms();
	if (texture.getObject()) {
		texture->bind();
		gears.game->currentShader->setUniform("uTextureCount", 1);
	} else {
		gears.game->currentShader->setUniform("uTextureCount", 0);
	}
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
}


void gGame::updateShaderUniforms() {
	if (worldMatDirty) {
		worldMatDirty = false;
		gears.game->currentShader->setWorldMatrix(worldMatrixStack[currentStack]);
	}
}

void gGame::setShader(gShader* shader) {
	currentShader = shader;
	currentShader->begin();
	updateProjectionMatrix();
	updateViewMatrix();
	currentShader->setColor(Vec4(1.0, 1.0f, 1.0f, 1.0f));
	worldMatDirty = true;
}
