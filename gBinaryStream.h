#pragma once

#ifndef G_BINARY_STREAM_H__
#define G_BINARY_STREAM_H__

#include <fstream>
#include "gVec.h"
#include "gMat.h"

template <class T>
class gBinaryStream {
	T stream;
public:
	gBinaryStream() {}

	void open(std::string filename, int flags) {
		stream.open(filename, flags);
	}

	bool isOpen() {
		return stream.isOpen();
	}
	void close() {
		stream.close();
	}


	gBinaryStream<T>& operator<<(const int &val) {
		stream.write((const char*)&val, sizeof(int));
		return *this;
	}
	gBinaryStream<T>& operator<<(const unsigned &val) {
		stream.write((const char*)&val, sizeof(unsigned));
		return *this;
	}
	gBinaryStream<T>& operator<<(const float &val) {
		stream.write((const char*)&val, sizeof(float));
		return *this;
	}
	gBinaryStream<T>& operator<<(const double &val) {
		stream.write((const char*)&val, sizeof(double));
		return *this;
	}


	gBinaryStream<T>& operator<<(const Vec2 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const Vec3 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const Vec4 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}

	gBinaryStream<T>& operator<<(const IntVec2 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const IntVec3 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const IntVec4 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const DoubleVec2 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const DoubleVec3 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const DoubleVec4 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const LongVec2 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const LongVec3 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const LongVec4 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const Mat2 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const Mat3 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator<<(const Mat4 &val) {
		stream.write((const char*)&val, sizeof(val));
		return *this;
	}
	void writeBinary(const char* data, int size) {
		stream.write(data, size);
	}



	gBinaryStream<T>& operator>>(const int &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const unsigned &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const float &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const double &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}


	gBinaryStream<T>& operator>>(const Vec2 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const Vec3 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const Vec4 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}

	gBinaryStream<T>& operator>>(const IntVec2 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const IntVec3 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const IntVec4 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const DoubleVec2 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const DoubleVec3 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const DoubleVec4 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const LongVec2 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const LongVec3 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const LongVec4 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const Mat2 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const Mat3 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}
	gBinaryStream<T>& operator>>(const Mat4 &val) {
		stream.read((char*)&val, sizeof(val));
		return *this;
	}

	void readBinary(char* data, int size) {
		stream.read(data, size);
	}
};


typedef gBinaryStream<std::ifstream> gBinaryFileInputStream;

typedef gBinaryStream<std::ofstream> gBinaryFileOutputStream;


#endif