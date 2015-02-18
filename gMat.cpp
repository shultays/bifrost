#include "gMat.h"

Vec4 operator*(const Vec4 &v, const Mat4 &m){
	Vec4 result;

	result[0] = v[0] * m._00 + v[1] * m._10 + v[2] * m._20 + v[3] * m._30;
	result[1] = v[0] * m._01 + v[1] * m._11 + v[2] * m._21 + v[3] * m._31;
	result[2] = v[0] * m._02 + v[1] * m._12 + v[2] * m._22 + v[3] * m._32;
	result[3] = v[0] * m._03 + v[1] * m._13 + v[2] * m._23 + v[3] * m._33;

	return result;
}

Vec4 &operator *= (Vec4 &v, const Mat4 &m){
	v = v*m;
	return v;
}