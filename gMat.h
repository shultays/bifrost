#pragma once

#ifndef G_MAT_H__
#define G_MAT_H__

#include "gVec.h"
#include <math.h>

class Mat4;
Vec4 operator*(const Vec4 &v, const Mat4 &m);
Vec4 &operator*=(Vec4 &v, const Mat4 &m);


class Mat4{
public:
	union{
		struct{
			float arr[4][4];
		};
		struct{
			float data[4*4];
		};
		struct{
			Vec4 row0;
			Vec4 row1;
			Vec4 row2;
			Vec4 row3;
		};
		struct{
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};

	};

	Mat4(){
	}

	Mat4(float con){
		for (int i = 0; i < 4 * 4; i++)
			data[i] = con;
	}

	Mat4(Vec4 v0, Vec4 v1, Vec4 v2, Vec4 v3){
		row0 = v0;
		row1 = v1;
		row2 = v2;
		row3 = v3;
	}
	static Mat4 identity(){
		Mat4 mat;
		mat.makeIdentity();
		return mat;
	}

	void makeIdentity(){
		_00 = 1.0f;
		_01 = 0.0f;
		_02 = 0.0f;
		_03 = 0.0f;
		_10 = 0.0f;
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_20 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_30 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
	}


	static Mat4 rotation(float angle, const Vec3& axis){
		float s = sin(angle);
		float c = cos(angle);
		float s_1 = 1.0f - s;
		float c_1 = 1.0f - s;

		Mat4 mat;

		mat._00 = axis[0] * axis[0] * c_1 + c;
		mat._01 = axis[1] * axis[0] * c_1 - axis[2] * s;
		mat._02 = axis[2] * axis[0] * c_1 + axis[1] * s;
		mat._03 = 0.0f;
		mat._10 = axis[0] * axis[1] * c_1 + axis[2] * s;
		mat._11 = axis[1] * axis[1] * c_1 + c;
		mat._12 = axis[2] * axis[1] * c_1 - axis[0] * s;
		mat._13 = 0.0f;
		mat._20 = axis[0] * axis[2] * c_1 - axis[1] * s;
		mat._21 = axis[1] * axis[2] * c_1 + axis[0] * s;
		mat._22 = axis[2] * axis[2] * c_1 + c;
		mat._23 = 0.0f;
		mat._30 = mat._31 = mat._32 = 0.0f;
		mat._33 = 1.0f;

		return mat;
	}


	static Mat4 rotationX(float angle){
		float s = sin(angle);
		float c = cos(angle);
		float s_1 = 1.0f - s;
		float c_1 = 1.0f - s;

		Mat4 mat;

		mat._00 = c_1 + c;
		mat._01 = 0.0f;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = c;
		mat._12 = -s;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = s;
		mat._22 = c;
		mat._23 = 0.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = 1.0f;

		return mat;
	}

	static Mat4 rotationY(float angle){
		float s = sin(angle);
		float c = cos(angle);
		float s_1 = 1.0f - s;
		float c_1 = 1.0f - s;

		Mat4 mat;

		mat._00 = c;
		mat._01 = 0.0f;
		mat._02 = s;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = c_1 + c;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = - s;
		mat._21 = 0.0f;
		mat._22 = c;
		mat._23 = 0.0f;
		mat._23 = 0.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = 1.0f;

		return mat;
	}

	static Mat4 rotationZ(float angle){
		float s = sin(angle);
		float c = cos(angle);
		float s_1 = 1.0f - s;
		float c_1 = 1.0f - s;

		Mat4 mat;

		mat._00 = c;
		mat._01 = - s;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = s;
		mat._11 = c;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = 0.0f;
		mat._22 = c_1 + c;
		mat._23 = 0.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = 1.0f;


		return mat;
	}

	static Mat4 translate(const Vec3& vec){
		Mat4 mat;

		mat._00 = 1.0f;
		mat._01 = 0.0f;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = 1.0f;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = 0.0f;
		mat._22 = 1.0f;
		mat._23 = 0.0f;
		mat._30 = vec.x;
		mat._31 = vec.y;
		mat._32 = vec.z;
		mat._33 = 1.0f;

		return mat;
	}

	Mat4& translateBy(const Vec3& vec){
		*this = *this*Mat4::translate(vec);
		return *this;
	}



	Mat4& scaleBy(const Vec3& vec){
		_00 *= vec[0];
		_01 *= vec[1];
		_02 *= vec[2];

		_10 *= vec[0];
		_11 *= vec[1];
		_12 *= vec[2];

		_20 *= vec[0];
		_21 *= vec[1];
		_22 *= vec[2];

		_30 *= vec[0];
		_31 *= vec[1];
		_32 *= vec[2];

		return *this;
	}

	Mat4& scaleBy(float con){
		_00 *= con;
		_01 *= con;
		_02 *= con;

		_10 *= con;
		_11 *= con;
		_12 *= con;

		_20 *= con;
		_21 *= con;
		_22 *= con;

		_30 *= con;
		_31 *= con;
		_32 *= con;

		return *this;
	}

	Mat4& rotateBy(float angle, const Vec3& axis){
		*this *= Mat4::rotation(angle, axis);
		return *this;
	}
	
	Mat4& rotateByX(float angle){ //optimize
		*this *= Mat4::rotationX(angle);
		return *this;
	}
	Mat4& rotateByY(float angle){ //optimize
		*this *= Mat4::rotationY(angle);
		return *this;
	} 
	Mat4& rotateByZ(float angle){ //optimize
		*this *= Mat4::rotationZ(angle);
		return *this;
	}
	static Mat4 scale(const Vec3& vec){
		Mat4 mat;

		mat._00 = vec.x;
		mat._01 = 0.0f;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = vec.y;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = 0.0f;
		mat._22 = vec.z;
		mat._23 = 0.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = 1.0f;

		return mat;
	}

	static Mat4 scale(const float scale){
		Mat4 mat;

		mat._00 = scale;
		mat._01 = 0.0f;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = scale;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = 0.0f;
		mat._22 = scale;
		mat._23 = 0.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = 0.0f;
		mat._33 = 1.0f;

		return mat;
	}


	Mat4 transpose(){
		Mat4 mat;
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				mat.arr[i][j] = arr[j][i];
			}
		}
		return mat;
	}


	bool operator==(const Mat4& other){
		bool ret = true;
		for (int i = 0; i < 4*4 && ret; ++i)
			ret &= (other.data[i] == data[i]);
		return ret;
	}


	Mat4 Mat4::operator*(const Mat4 &other) const{
		Mat4 result;

		result._00 = _00 * other._00 + _01 * other._10 + _02 * other._20 + _03 * other._30;
		result._01 = _00 * other._01 + _01 * other._11 + _02 * other._21 + _03 * other._31;
		result._02 = _00 * other._02 + _01 * other._12 + _02 * other._22 + _03 * other._32;
		result._03 = _00 * other._03 + _01 * other._13 + _02 * other._23 + _03 * other._33;

		result._10 = _10 * other._00 + _11 * other._10 + _12 * other._20 + _13 * other._30;
		result._11 = _10 * other._01 + _11 * other._11 + _12 * other._21 + _13 * other._31;
		result._12 = _10 * other._02 + _11 * other._12 + _12 * other._22 + _13 * other._32;
		result._13 = _10 * other._03 + _11 * other._13 + _12 * other._23 + _13 * other._33;

		result._20 = _20 * other._00 + _21 * other._10 + _22 * other._20 + _23 * other._30;
		result._21 = _20 * other._01 + _21 * other._11 + _22 * other._21 + _23 * other._31;
		result._22 = _20 * other._02 + _21 * other._12 + _22 * other._22 + _23 * other._32;
		result._23 = _20 * other._03 + _21 * other._13 + _22 * other._23 + _23 * other._33;

		result._30 = _30 * other._00 + _31 * other._10 + _32 * other._20 + _33 * other._30;
		result._31 = _30 * other._01 + _31 * other._11 + _32 * other._21 + _33 * other._31;
		result._32 = _30 * other._02 + _31 * other._12 + _32 * other._22 + _33 * other._32;
		result._33 = _30 * other._03 + _31 * other._13 + _32 * other._23 + _33 * other._33;

		return result;
	}


	Mat4 &Mat4::operator *= (const Mat4 &m){
		*this = *this * m;
		return *this;
	}


	Vec4 operator*(const Vec4 &v) {
		Vec4 result;

		result[0] = v[0] * _00 + v[1] * _01 + v[2] * _02 + v[3] * _03;
		result[1] = v[0] * _10 + v[1] * _11 + v[2] * _12 + v[3] * _13;
		result[2] = v[0] * _20 + v[1] * _21 + v[2] * _22 + v[3] * _23;
		result[3] = v[0] * _30 + v[1] * _31 + v[2] * _32 + v[3] * _33;

		return result;
	}

	static Mat4 perspective(float zNear, float zFar, float povInRadian, float width, float height) {
		float h = cos(0.5f * povInRadian) / sin(0.5f * povInRadian);
		float w = h * height / width; 
		float zRange = zFar - zNear;
		
		Mat4 mat;
		mat._00 = w;
		mat._01 = 0.0f;
		mat._02 = 0.0f;
		mat._03 = 0.0f;
		mat._10 = 0.0f;
		mat._11 = h;
		mat._12 = 0.0f;
		mat._13 = 0.0f;
		mat._20 = 0.0f;
		mat._21 = 0.0f;
		mat._22 = -(zNear +zFar) / zRange;
		mat._23 = -1.0f;
		mat._30 = 0.0f;
		mat._31 = 0.0f;
		mat._32 = -2.0f * zFar * zNear / zRange;
		mat._33 = 0.0f;
		return mat;
	}

	Mat4 inverse() const{
		Mat4 ret;
		float det;

		ret.data[0] = data[5] * data[10] * data[15] -
			data[5] * data[11] * data[14] -
			data[9] * data[6] * data[15] +
			data[9] * data[7] * data[14] +
			data[13] * data[6] * data[11] -
			data[13] * data[7] * data[10];

		ret.data[4] = -data[4] * data[10] * data[15] +
			data[4] * data[11] * data[14] +
			data[8] * data[6] * data[15] -
			data[8] * data[7] * data[14] -
			data[12] * data[6] * data[11] +
			data[12] * data[7] * data[10];

		ret.data[8] = data[4] * data[9] * data[15] -
			data[4] * data[11] * data[13] -
			data[8] * data[5] * data[15] +
			data[8] * data[7] * data[13] +
			data[12] * data[5] * data[11] -
			data[12] * data[7] * data[9];

		ret.data[12] = -data[4] * data[9] * data[14] +
			data[4] * data[10] * data[13] +
			data[8] * data[5] * data[14] -
			data[8] * data[6] * data[13] -
			data[12] * data[5] * data[10] +
			data[12] * data[6] * data[9];

		ret.data[1] = -data[1] * data[10] * data[15] +
			data[1] * data[11] * data[14] +
			data[9] * data[2] * data[15] -
			data[9] * data[3] * data[14] -
			data[13] * data[2] * data[11] +
			data[13] * data[3] * data[10];

		ret.data[5] = data[0] * data[10] * data[15] -
			data[0] * data[11] * data[14] -
			data[8] * data[2] * data[15] +
			data[8] * data[3] * data[14] +
			data[12] * data[2] * data[11] -
			data[12] * data[3] * data[10];

		ret.data[9] = -data[0] * data[9] * data[15] +
			data[0] * data[11] * data[13] +
			data[8] * data[1] * data[15] -
			data[8] * data[3] * data[13] -
			data[12] * data[1] * data[11] +
			data[12] * data[3] * data[9];

		ret.data[13] = data[0] * data[9] * data[14] -
			data[0] * data[10] * data[13] -
			data[8] * data[1] * data[14] +
			data[8] * data[2] * data[13] +
			data[12] * data[1] * data[10] -
			data[12] * data[2] * data[9];

		ret.data[2] = data[1] * data[6] * data[15] -
			data[1] * data[7] * data[14] -
			data[5] * data[2] * data[15] +
			data[5] * data[3] * data[14] +
			data[13] * data[2] * data[7] -
			data[13] * data[3] * data[6];

		ret.data[6] = -data[0] * data[6] * data[15] +
			data[0] * data[7] * data[14] +
			data[4] * data[2] * data[15] -
			data[4] * data[3] * data[14] -
			data[12] * data[2] * data[7] +
			data[12] * data[3] * data[6];

		ret.data[10] = data[0] * data[5] * data[15] -
			data[0] * data[7] * data[13] -
			data[4] * data[1] * data[15] +
			data[4] * data[3] * data[13] +
			data[12] * data[1] * data[7] -
			data[12] * data[3] * data[5];

		ret.data[14] = -data[0] * data[5] * data[14] +
			data[0] * data[6] * data[13] +
			data[4] * data[1] * data[14] -
			data[4] * data[2] * data[13] -
			data[12] * data[1] * data[6] +
			data[12] * data[2] * data[5];

		ret.data[3] = -data[1] * data[6] * data[11] +
			data[1] * data[7] * data[10] +
			data[5] * data[2] * data[11] -
			data[5] * data[3] * data[10] -
			data[9] * data[2] * data[7] +
			data[9] * data[3] * data[6];

		ret.data[7] = data[0] * data[6] * data[11] -
			data[0] * data[7] * data[10] -
			data[4] * data[2] * data[11] +
			data[4] * data[3] * data[10] +
			data[8] * data[2] * data[7] -
			data[8] * data[3] * data[6];

		ret.data[11] = -data[0] * data[5] * data[11] +
			data[0] * data[7] * data[9] +
			data[4] * data[1] * data[11] -
			data[4] * data[3] * data[9] -
			data[8] * data[1] * data[7] +
			data[8] * data[3] * data[5];

		ret.data[15] = data[0] * data[5] * data[10] -
			data[0] * data[6] * data[9] -
			data[4] * data[1] * data[10] +
			data[4] * data[2] * data[9] +
			data[8] * data[1] * data[6] -
			data[8] * data[2] * data[5];

		det = data[0] * ret.data[0] + data[1] * ret.data[4] + data[2] * ret.data[8] + data[3] * ret.data[12];

		if (det == 0)
			return identity();

		det = 1.0f / det;

		for (int i = 0; i < 16; i++)
			ret.data[i] = ret.data[i] * det;

		return ret;
	}

	static Mat4 lookAt(const Vec3& eye, const Vec3& focus, const Vec3& up){
		/*Vec3 t = eye - focus;

		Vec3 vz = Vec3::normalize(eye - focus);
		Vec3 vx = Vec3::normalize(Vec3::cross(up, vz));
		// vy doesn't need to be normalized because it's a cross
		// product of 2 normalized vectors
		Vec3 vy = Vec3::cross(vz, vx);
		Mat4 inverseViewMatrix = Mat4(Vec4(vx, 0),
			Vec4(vy, 0),
			Vec4(vz, 0),
			Vec4(eye, 1));

		
		inverseViewMatrix = inverseViewMatrix.inverse();
		*/
		Vec3 Z = eye-focus;
		Z.normalize();

		Vec3 Y = up;
		Vec3 X = Y.cross(Z);
		Y = Z.cross(X);

		X.normalize();
		Y.normalize();

		Mat4 mat;


		mat._00 = X.x;
		mat._10 = X.y;
		mat._20 = X.z;
		mat._30 = -X.dot(eye);
		mat._01 = Y.x;
		mat._11 = Y.y;
		mat._21 = Y.z;
		mat._31 = -Y.dot(eye);
		mat._02 = Z.x;
		mat._12 = Z.y;
		mat._22 = Z.z;
		mat._32 = -Z.dot(eye);
		mat._03 = 0;
		mat._13 = 0;
		mat._23 = 0;
		mat._33 = 1.0f;



		return mat;
	}
};


#endif