#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "gTools.h"
#include "gBinaryStream.h"

template <class T>
class Grid {
public:
	T* data;
	int w, h;

	Grid(int s) {
		data = NULL;
		init(s, s);
	}

	Grid(int w, int h) {
		data = NULL;
		init(w, h);
	}

	Grid() {
		data = NULL;
	}

	void init(int w, int h, T data) {
		init(w, h);
		setAll(data);
	}
	void init(int w, int h) {
		SAFE_DELETE(data);
		this->w = w;
		this->h = h;
		data = new T[w*h];
	}
	void init(int s) {
		init(s, s);
	}
	void setAll(T a) {
		T* p = data + w*h;
		while (p-- > data) *p = a;
	}

	bool isValid(int x, int y) {
		return data && x >= 0 && y >= 0 && x < w && y < h;
	}

	bool isValid(const IntVec2& side) {
		return isValid(side.x, side.y);
	}


	void setZero() {
		memset(data, 0, sizeof(T)*w*h);
	}

	inline T& operator[](const IntVec2& v) const {
		return *(data + v.x*h + v.y);
	}


	inline T* operator[](int i) const {
		return data + i*h;
	}

	~Grid() {
		SAFE_DELETE(data);
	}

	void serialize(gBinaryFileOutputStream& output) {
		assert(data);
		output << w;
		output << h;
		output.writeBinary((const char*)data, sizeof(T)*w*h);
	}

	void deserialize(gBinaryFileInputStream& input) {
		SAFE_DELETE(data);
		input >> w;
		input >> h;
		init(w, h);
		input.readBinary((char*)data, sizeof(T)*w*h);
	}

	inline T& atRawIndex(int i) const {
		return *(data + i);
	}

};

#endif

