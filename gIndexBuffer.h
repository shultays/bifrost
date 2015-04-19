#pragma once

#ifndef G_INDEX_BUFFER_H__
#define G_INDEX_BUFFER_H__

#include "gTools.h"

class gIndexBuffer {
	GLuint gl_buffer;
	byte *buffer;
	unsigned *bufferAsBig;
	unsigned short *bufferAsSmall;

protected:
	bool isStatic;
	bool isBig;

	unsigned indexCount;
	unsigned maxIndexCount;

	bool built;
	unsigned indexSize;

	int primitiveType;

	unsigned getStartPositionAt(unsigned i) {
		return i*indexSize;
	}

public:
	gIndexBuffer(bool isStatic = true) {
		built = false;
		buffer = NULL;
		maxIndexCount = 0;
		indexSize = 0;
		indexCount = 0;
		this->isStatic = isStatic;
	}

	gIndexBuffer(unsigned maxIndexCount, int primitiveType = GL_TRIANGLES, bool isStatic = true);
	~gIndexBuffer();

	void addIndex(unsigned val);
	void removeIndexAt(unsigned i);
	void setIndexAt(unsigned i, unsigned val);
	unsigned getIndexAt(unsigned i);
	void build();

	int getGlType();
	void bind();

	void drawAll();
};


#endif