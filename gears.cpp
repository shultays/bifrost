#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gTools.h"
#include "gGears.h"
#include "gGlobals.h"
#include "Valhalla.h"


void key( GLFWwindow* window, int k, int s, int action, int mods )
{
	gears.key(k, s, action, mods);
}


void reshape( GLFWwindow* window, int width, int height )
{
	gears.resize(width, height);
}

int main(int argc, char *argv[])
{
	GLFWwindow* window;
	int width, height;
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);

    window = glfwCreateWindow( 600, 480, "Gears", NULL, NULL );
    if (!window)
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

	glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );

    glfwGetFramebufferSize(window, &width, &height);

	gears.window = window;
	gears.width = width;
	gears.height = height;

	gears.game = new Valhalla();

    reshape(window, width, height);


    gears.init(argc, argv);

    while( !glfwWindowShouldClose(window) )
    {
        gears.tick();

        glfwPollEvents();
    }

    glfwTerminate();

    exit( EXIT_SUCCESS );
}

