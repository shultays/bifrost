#ifndef G_TOOLS_H
#define G_TOOLS_H

#include <assert.h>
#include <GL\glew.h>
#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>
#include <string>


#include "gVec.h"
#include "gMat.h"

#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062

#define SAFE_DELETE(x) do{if(x){delete x; x = NULL;}}while(0)
#define swapt(x, y) do{auto t=x; x=y; y=t;}while(0) 
#define vec3_to_3_float(vec) vec.x, vec.y, vec.z


#define lerp(x, y, a) (x*(1-a)+y*a)


typedef unsigned char byte;

const float pi = (float)E_PI;
const float pi_2 = (float)(E_PI * 2.0);
const float pi_d2 = (float)(E_PI * 0.5);
const float pi_3d2 = (float)(E_PI * 1.5);

const double d_pi = E_PI;
const double d_pi_2 = (E_PI * 2.0);
const double d_pi_d2 = (E_PI * 0.5);
const double d_pi_3d2 = (E_PI * 1.5);

inline float degreeToRadian(float deg) {
	return deg * pi / 180.0f;
}

inline float radianToDegree(float rad) {
	return rad * 180.0f / pi;
}

inline double degreeToRadian(double deg) {
	return deg * d_pi / 180.0;
}

inline double radianToDegree(double rad) {
	return rad * 180.0 / d_pi;
}

bool textFileRead(char *fileName, std::string &data);

Vec3 lerpColor(const Vec3& c0, const Vec3& c1, float a);
Vec2 lerpVec(const Vec2& v0, const Vec2& v1, float a);
Vec3 lerpVec(const Vec3& v0, const Vec3& v1, float a);
Vec4 lerpVec(const Vec4& v0, const Vec4& v1, float a);

Vec3 rgbToHsv(const Vec3& color);
Vec3 hsvToRgb(const Vec3& color);

#define gmin(x, y) (x>y?y:x)
#define gmin3(x, y, z) (x>y?gmin(z,y):gmin(x,y))
#define gmin4(x, y, z, a) (x>y?gmin3(y, z, a):gmin3(x, z, a))

#define gmax(x, y) (x<y?y:x)
#define gmax3(x, y, z) (x<y?gmin(z,y):gmin(x,y))
#define gmax4(x, y, z, a) (x<y?gmin3(y, z, a):gmin3(x, z, a))



#endif