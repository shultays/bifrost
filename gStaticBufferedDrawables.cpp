#include "gStaticBufferedDrawables.h"
#include "gGlobals.h"


void gStaticIndexBufferedDrawable::render() {
	gStaticVertexBuffer::bind();
	gStaticIndexBuffer::bind();

	gStaticIndexBuffer::drawAll();
}

void gStaticTriangleVertexBufferDrawable::render() {
	bind();
	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
}