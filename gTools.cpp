#include "gTools.h"

#include <stdio.h>
#include <fstream>
#include <streambuf>

#ifdef WINDOWS
#undef APIENTRY
#include "windows.h"
#endif

bool textFileRead(char *fileName, std::string &data) {
	std::ifstream t(fileName);
	if (!t.good()) {
		return false;
	}
	t.seekg(0, std::ios::end);
	data.reserve((unsigned int)t.tellg());
	t.seekg(0, std::ios::beg);

	data.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return true;
}

Vec3 rgbToHsv(const Vec3& color) {
	float min, max, delta;
	Vec3 ret;
	min = gmin3(color.r, color.g, color.b);
	max = gmax3(color.r, color.b, color.b);


	ret.z = max;

	delta = max - min;

	if (max != 0.0f)
		ret.y = delta / max;
	else {
		ret.y = 0;
		ret.x = -1;
		return ret;
	}

	if (color.r == max)
		ret.x = (color.g - color.b) / delta;
	else if (color.g == max)
		ret.x = 2 + (color.b - color.r) / delta;
	else
		ret.x = 4 + (color.r - color.g) / delta;


	if (ret.x < 0)
		ret.x += 6;

	return ret;
}
Vec3 hsvToRgb(const Vec3& color) {
	int i;
	float f, p, q, t;
	Vec3 ret;
	if (color.y == 0) {
		ret = color.z;
		return ret;
	}

	i = (int)floor(color.x);
	f = color.x - i;
	p = color.z * (1 - color.y);
	q = color.z * (1 - color.y * f);
	t = color.z * (1 - color.y * (1 - f));

	switch (i) {
		case 0:
			ret.r = color.z;
			ret.g = t;
			ret.b = p;
			break;
		case 1:
			ret.r = q;
			ret.g = color.z;
			ret.b = p;
			break;
		case 2:
			ret.r = p;
			ret.g = color.z;
			ret.b = t;
			break;
		case 3:
			ret.r = p;
			ret.g = q;
			ret.b = color.z;
			break;
		case 4:
			ret.r = t;
			ret.g = p;
			ret.b = color.z;
			break;
		default:
			ret.r = color.z;
			ret.g = p;
			ret.b = q;
			break;
	}
	return ret;
}


Vec2 lerpVec(const Vec2& v0, const Vec2& v1, float a) {
	float a_1 = 1.0f - a;
	return Vec2(v0.x*a_1 + v1.x*a, v0.y*a_1 + v1.y*a);

}
Vec3 lerpVec(const Vec3& v0, const Vec3& v1, float a) {
	float a_1 = 1.0f - a;
	return Vec3(v0.x*a_1 + v1.x*a, v0.y*a_1 + v1.y*a, v0.z*a_1 + v1.z*a);

}
Vec4 lerpVec(const Vec4& v0, const Vec4& v1, float a) {
	float a_1 = 1.0f - a;
	return Vec4(v0.x*a_1 + v1.x*a, v0.y*a_1 + v1.y*a, v0.z*a_1 + v1.z*a, v0.o*a_1 + v1.o*a);

}


Vec3 lerpColor(const Vec3& c0, const Vec3& c1, float a) {
	Vec3 h0 = rgbToHsv(c0);
	Vec3 h1 = rgbToHsv(c1);

	Vec3 v = lerpVec(h0, h1, a);

	return hsvToRgb(v);
}


void sleepMS(int ms) {
#ifdef WINDOWS
	Sleep(ms);
#endif
}