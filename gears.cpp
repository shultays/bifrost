#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gTools.h"
#include "gGears.h"
#include "gGlobals.h"
#include "Valhalla.h"


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
	printf("%lf %lf\n", xoffset, yoffset);
}


void reshape(GLFWwindow* window, int width, int height) {
	gears.resize(width, height);
}

int main(int argc, char *argv[]) {
	GLFWwindow* window;
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

	gears.game = new Valhalla();

	reshape(window, width, height);


	gears.init(argc, argv);

	while (!glfwWindowShouldClose(window)) {
		gears.tick();

		glfwPollEvents();
	}

	glfwTerminate();

	exit(EXIT_SUCCESS);
}

