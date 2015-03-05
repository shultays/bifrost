#include "gStaticIndexBuffer.h"

gStaticIndexBuffer::gStaticIndexBuffer(unsigned elementCount, int primitiveType) {
	this->elementCount = elementCount;
	this->primitiveType = primitiveType;
	isBig = elementCount > 0xFFFF;
	elementSize = isBig ? 4 : 2;

	buffer = new byte[getStartPositionAt(elementCount)];
	bufferAsBig = (unsigned*)buffer;
	bufferAsSmall = (unsigned short*)buffer;

	built = false;
}

gStaticIndexBuffer::~gStaticIndexBuffer() {
	SAFE_DELETE_ARR(buffer);
	if (built) {
		glDeleteBuffers(1, &gl_buffer);
		built = false;
	}
}

void gStaticIndexBuffer::setIndexAt(unsigned i, unsigned val) {
	assert(!built);
	assert(buffer);
	assert(i >= 0 && i < elementCount);
	if (isBig) {
		bufferAsBig[i] = val;
	} else {
		bufferAsSmall[i] = val;
	}
}

unsigned gStaticIndexBuffer::getIndexAt(unsigned i) {
	assert(!built);
	assert(buffer);
	assert(i >= 0 && i < elementCount);
	if (isBig) {
		return bufferAsBig[i];
	} else {
		return bufferAsSmall[i];
	}
}

void gStaticIndexBuffer::build() {
	assert(!built);
	assert(buffer);
	built = true;
	glGenBuffers(1, &gl_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getStartPositionAt(elementCount), buffer, GL_STATIC_DRAW);

	SAFE_DELETE_ARR(buffer);
}

int gStaticIndexBuffer::getGlType() {
	return isBig ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
}

void gStaticIndexBuffer::bind() {
	assert(built);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer);
}

void gStaticIndexBuffer::drawAll() {
	assert(built);
	glDrawElements(
		primitiveType,
		elementCount,
		getGlType(),
		(void*)0
		);
}

