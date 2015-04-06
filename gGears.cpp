#include "gGears.h"
#include "gGlobals.h"

void gGears::init(int argc, char *argv[]) {
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	slaveController.startSlaves(3);
	game->init();
}

void gGears::resize(int width, int height) {
	this->width = width;
	this->height = height;
	glViewport(0, 0, (GLint)width, (GLint)height);
	game->resize(width, height);
}

void gGears::tick() {
	time.tick();

	float t = time.getTime();

	time.fixed_dt = t - lastUpdateTime;
	time.dt = t - lastDrawTime;

	if (t - lastUpdateTime >= update_interval) {
		input.informListeners();
		game->update(time.fixed_dt);
		input.clearOneTickStates();
		lastUpdateTime = t;
	}

	if (t - lastDrawTime >= draw_interval) {
		game->tick(time.dt);
		game->render();
		lastDrawTime = t;
		glfwSwapBuffers(window);
	}
	slaveController.update();
	t = time.getTime();
	float timeToSleep = gmin(update_interval - (t - lastUpdateTime), draw_interval - (t - lastDrawTime));
	if (timeToSleep > 10) {
		sleepMS((int)(timeToSleep * 1000));
	}
}

void gGears::addSlaveWork(gSlaveWork* slaveWork) {
	slaveController.addWork(slaveWork);
}
void gGears::key(int k, int s, int action, int mods) {
	if (k < 0 || k >= KEY_COUNT) {
		return;
	}
	switch (action) {
		case GLFW_RELEASE:
			input.keyRelease(time.getTime(), k);
			break;
		case GLFW_PRESS:
			input.keyPress(time.getTime(), k);
			break;
		case GLFW_REPEAT:
			input.keyType(time.getTime(), k);
			break;
		default:
			break;
	}
}


void gGears::mouseMove(double x, double y) {
	input.mouseMove(Vec2((float)x, (float)y));
}
void gGears::scroll(double x, double y) {
	input.scroll(Vec2((float)x, (float)y));

}