#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gears.h"
#include "gTools.h"
#include "gGears.h"
#include "gGlobals.h"


GLFWwindow* window;

void key(GLFWwindow* window, int k, int s, int action, int mods) {
	gears.key(k, s, action, mods);
}


void mouse(GLFWwindow* window, int k, int action, int mods) {
	gears.key(k + MOUSE_BUTTON_1 - GLFW_MOUSE_BUTTON_1, 0, action, mods);
}

void mouseMove(GLFWwindow* window, double x, double y) {
	//glfwSetCursorPos(window, gears.width / 2, gears.height / 2);
	gears.mouseMove(x, y);

}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
	gears.scroll(xoffset, yoffset);
}


void reshape(GLFWwindow* window, int width, int height) {
	gears.resize(width, height);
}

int startGears(int argc, char *argv[], gGame* game) {
	int width, height;

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_DEPTH_BITS, 16);

	window = glfwCreateWindow(600, 480, "Gears", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetKeyCallback(window, key);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetScrollCallback(window, scroll);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize glew\n");
		exit(EXIT_FAILURE);
	}


	glfwGetFramebufferSize(window, &width, &height);

	gears.window = window;
	gears.width = width;
	gears.height = height;

	gears.game = game;

	reshape(window, width, height);


	gears.init(argc, argv);

	return 0;
}

void gearsLoop() {
	while (!glfwWindowShouldClose(window)) {
		gears.tick();

		glfwPollEvents();
	}
}

void terminateGears() {
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

