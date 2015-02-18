#ifndef G_TOOLS_H
#define G_TOOLS_H


#include <assert.h>
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>
#include "gVec.h"
#include "gMat.h"

#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

const float pi = (float)E_PI;
const float pi_2 = (float)(E_PI * 2.0);
const float pi_d2 = (float)(E_PI * 0.5);
const float pi_3d2 = (float)(E_PI * 1.5);

const double d_pi = E_PI;
const double d_pi_2 = (E_PI * 2.0);
const double d_pi_d2 = (E_PI * 0.5);
const double d_pi_3d2 = (E_PI * 1.5);

inline float degreeToRadian(float deg){
	return deg * pi / 180.0f;
}

inline float radianToDegree(float rad){
	return rad * 180.0f / pi;
}

inline double degreeToRadian(double deg){
	return deg * d_pi / 180.0;
}

inline double radianToDegree(double rad){
	return rad * 180.0 / d_pi;
}
#define swapt(x, y) do{auto t=x; x=y; y=t;}while(0) 
#define vec3_to_3_float(vec) vec.x, vec.y, vec.z

#endif