#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdlib.h>
#include <string.h>
#include "gTools.h"

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

	inline T* operator[](int i) const {
		return data + i*h;
	}

	~Grid() {
		SAFE_DELETE(data);
	}
};

#endif
