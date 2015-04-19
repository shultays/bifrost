#include "gIndexBuffer.h"

gIndexBuffer::gIndexBuffer(unsigned maxIndexCount, int primitiveType, bool isStatic) {
	this->maxIndexCount = maxIndexCount;
	this->primitiveType = primitiveType;
	isBig = maxIndexCount > 0xFFFF;
	indexSize = isBig ? 4 : 2;

	buffer = new byte[getStartPositionAt(maxIndexCount)];

	bufferAsBig = (unsigned*)buffer;
	bufferAsSmall = (unsigned short*)buffer;

	indexCount = 0;
	built = false;
	this->isStatic = isStatic;
}

gIndexBuffer::~gIndexBuffer() {
	SAFE_DELETE_ARR(buffer);
	if (built) {
		glDeleteBuffers(1, &gl_buffer);
		built = false;
	}
}

void gIndexBuffer::addIndex(unsigned val) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(indexCount < maxIndexCount);
	if (isBig) {
		bufferAsBig[indexCount++] = val;
	} else {
		bufferAsSmall[indexCount++] = val;
	}
}

void gIndexBuffer::removeIndexAt(unsigned i) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < maxIndexCount && indexCount > 0);
	if (isBig) {
		bufferAsBig[i] = bufferAsBig[--indexCount];
	} else {
		bufferAsSmall[i] = bufferAsSmall[--indexCount];
	}
}

void gIndexBuffer::setIndexAt(unsigned i, unsigned val) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < maxIndexCount);
	if (isBig) {
		bufferAsBig[i] = val;
	} else {
		bufferAsSmall[i] = val;
	}
}

unsigned gIndexBuffer::getIndexAt(unsigned i) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < maxIndexCount);
	if (isBig) {
		return bufferAsBig[i];
	} else {
		return bufferAsSmall[i];
	}
}

void gIndexBuffer::build() {
	assert(!built || !isStatic);
	assert(buffer);
	if (!built) {
		built = true;
		glGenBuffers(1, &gl_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getStartPositionAt(maxIndexCount), buffer, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getStartPositionAt(maxIndexCount), buffer);
	}

	if (isStatic) {
		SAFE_DELETE_ARR(buffer);
	}
}

int gIndexBuffer::getGlType() {
	return isBig ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
}

void gIndexBuffer::bind() {
	assert(built || !isStatic);
	if (built) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
	}
}

void gIndexBuffer::drawAll() {
	assert(built || !isStatic);
	if (built) {
		unsigned drawCount = isStatic ? maxIndexCount : indexCount;
		if (drawCount) {
			glDrawElements(
				primitiveType,
				isStatic ? maxIndexCount : indexCount,
				getGlType(),
				(void*)0
				);
		}
	}
}

