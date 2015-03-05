#include "gStaticBufferedDrawables.h"
#include "gGlobals.h"


void gStaticIndexBufferedDrawable::render() {
	gears.game->updateShaderUniforms();

	gStaticVertexBuffer::bind();
	gStaticIndexBuffer::bind();

	gStaticIndexBuffer::drawAll();
}

void gStaticTriangleVertexBufferDrawable::render() {
	gears.game->updateShaderUniforms();

	bind();

	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
}