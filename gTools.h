
#include <assert.h>
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
using namespace glm;

#define swapt(x, y) do{auto t=x; x=y; y=t;}while(0) 
#define vec3_to_3_float(vec) vec.x, vec.y, vec.z

