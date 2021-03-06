#include "gIndexBufferedDrawable.h"
#include "gGlobals.h"


void gIndexBufferedDrawable::render() {
	gVertexBuffer::bind();
	gIndexBuffer::bind();

	gIndexBuffer::drawAll();
}

void gVertexBufferRenderable::render() {
	if (inited && built) {
		bind();
		glDrawArrays(primitiveType, 0, this->maxVertexCount);
	}
}