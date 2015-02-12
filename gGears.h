#ifndef G_GEARS_H__
#define G_GEARS_H__

#include "gTools.h"
#include "gInput.h"
#include "gGame.h"
#include "gTime.h"

const float update_interval = 0.01f;
const float draw_interval = 1.0f/60.0f;


class gGears{
public:
	GLFWwindow* window;
	int width, height;

	gGame *game;

	float lastDrawTime = 0.0f;
	float lastUpdateTime = 0.0f;

	void init(int argc, char *argv[]);
	void resize(int width, int height);
	void tick();
	void key(int k, int s, int action, int mods);
};

#endif