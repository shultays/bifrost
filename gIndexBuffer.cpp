#include "gIndexBuffer.h"

gIndexBuffer::gIndexBuffer(unsigned elementCount, int primitiveType, bool isStatic) {
	this->elementCount = elementCount;
	this->primitiveType = primitiveType;
	isBig = elementCount > 0xFFFF;
	elementSize = isBig ? 4 : 2;

	buffer = new byte[getStartPositionAt(elementCount)];
	bufferAsBig = (unsigned*)buffer;
	bufferAsSmall = (unsigned short*)buffer;
	totalIndex = 0;
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
	assert(totalIndex < elementCount);
	if (isBig) {
		bufferAsBig[totalIndex++] = val;
	} else {
		bufferAsSmall[totalIndex++] = val;
	}
}

void gIndexBuffer::removeIndexAt(unsigned i) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < elementCount && totalIndex > 0);
	if (isBig) {
		bufferAsBig[i] = bufferAsBig[--totalIndex];
	} else {
		bufferAsSmall[i] = bufferAsSmall[--totalIndex];
	}
}

void gIndexBuffer::setIndexAt(unsigned i, unsigned val) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < elementCount);
	if (isBig) {
		bufferAsBig[i] = val;
	} else {
		bufferAsSmall[i] = val;
	}
}

unsigned gIndexBuffer::getIndexAt(unsigned i) {
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < elementCount);
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getStartPositionAt(elementCount), buffer, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getStartPositionAt(elementCount), buffer);
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
		unsigned drawCount = isStatic ? elementCount : totalIndex;
		if (drawCount) {
			glDrawElements(
				primitiveType,
				isStatic ? elementCount : totalIndex,
				getGlType(),
				(void*)0
				);
		}
	}
}

