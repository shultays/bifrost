#pragma once

#ifndef G_STATIC_BUFFERED_DRAWABLE
#define G_STATIC_BUFFERED_DRAWABLE

#include "gStaticVertexBuffer.h"
#include "gStaticIndexBuffer.h"
#include "gGame.h"

class gStaticTriangleVertexBufferDrawable : public gStaticVertexBuffer, public gRenderable {
public:
	gStaticTriangleVertexBufferDrawable() : gRenderable(false) {}
	gStaticTriangleVertexBufferDrawable(int props, unsigned vertexCount, bool autoAdd = true) : gStaticVertexBuffer(props, vertexCount), gRenderable(autoAdd) {}

	virtual void render() override;
};

class gStaticIndexBufferedDrawable : public gStaticVertexBuffer, public gStaticIndexBuffer, public gRenderable {
public:
	gStaticIndexBufferedDrawable() : gRenderable(false) {}
	gStaticIndexBufferedDrawable(int props, unsigned vertexCount, unsigned indexCount, bool autoAdd = true) : gStaticVertexBuffer(props, vertexCount), gStaticIndexBuffer(indexCount), gRenderable(autoAdd) {}

	void build() {
		gStaticVertexBuffer::build();
		gStaticIndexBuffer::build();
	}

	virtual void render() override;
};


#endif