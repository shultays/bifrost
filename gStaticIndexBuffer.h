#pragma once

#ifndef G_STATIC_INDEX_BUFFER_H__
#define G_STATIC_INDEX_BUFFER_H__

#include "gTools.h"

class gStaticIndexBuffer {
	GLuint gl_buffer;
	byte *buffer;
	unsigned *bufferAsBig;
	unsigned short *bufferAsSmall;

protected:
	unsigned elementCount;
	bool isBig;

	bool built;
	unsigned elementSize;

	int primitiveType;

	unsigned getStartPositionAt(unsigned i) {
		return i*elementSize;
	}

public:
	gStaticIndexBuffer() {
		built = false;
		buffer = NULL;
		elementCount = 0;
		elementSize = 0;
	}

	gStaticIndexBuffer(unsigned elementCount, int primitiveType = GL_TRIANGLES);
	~gStaticIndexBuffer();
	void setIndexAt(unsigned i, unsigned val);

	unsigned getIndexAt(unsigned i);
	void build();

	int getGlType();
	void bind();

	void drawAll();
};


#endif