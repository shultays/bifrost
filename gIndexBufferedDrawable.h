#pragma once

#ifndef G_STATIC_BUFFERED_RENDERABLE
#define G_STATIC_BUFFERED_RENDERABLE

#include "gVertexBuffer.h"
#include "gIndexBuffer.h"
#include "gGame.h"

class gVertexBufferRenderable : public gVertexBuffer, public gRenderable {
	int isStatic;
	int primitiveType;
public:
	gVertexBufferRenderable(int props, unsigned vertexCount, bool autoAdd = true, bool isStatic = true, int primitiveType = GL_TRIANGLES) : gVertexBuffer(props, vertexCount, isStatic), gRenderable(autoAdd) {
		this->isStatic = isStatic;
		this->primitiveType = primitiveType;
	}

	virtual void render() override;
};

class gIndexBufferedDrawable : public gVertexBuffer, public gIndexBuffer, public gRenderable {
	int isStatic;
public:
	gIndexBufferedDrawable(int props, unsigned vertexCount, unsigned indexCount, int primitiveType = GL_TRIANGLES, bool isStatic = true, bool autoAdd = true) : gVertexBuffer(props, vertexCount, isStatic), gIndexBuffer(indexCount, primitiveType, isStatic), gRenderable(autoAdd) {
		this->isStatic = isStatic;
	}

	void build() {
		gVertexBuffer::build();
		gIndexBuffer::build();
	}

	virtual void render() override;
};


class gLinePointer {
public:
	VertexPointer p0;
	VertexPointer p1;
	int lineIndex;
};

class gIndexBufferedLineDrawable : public gIndexBufferedDrawable {
protected:
	int isStatic;
	int maxLine;
	int lineCount;
public:
	gIndexBufferedLineDrawable(int props, unsigned maxLine, bool isStatic = true, bool autoAdd = true) : gIndexBufferedDrawable(props, maxLine * 2, maxLine * 2, GL_LINES, isStatic, autoAdd) {
		this->isStatic = isStatic;
		lineCount = 0;
		this->maxLine = maxLine;
	}

	gLinePointer addLine() {
		assert(lineCount < maxLine);
		int p0Index = lineCount * 2;
		addIndex(p0Index);
		addIndex(p0Index + 1);
		return getLine(lineCount++);
	}
	gLinePointer getLine(int lineIndex) {
		assert(lineIndex < lineCount);
		gLinePointer r;
		int p0Index = getIndexAt(lineIndex * 2);
		r.p0 = getVertexPointerAt(p0Index);
		r.p1 = getVertexPointerAt(p0Index + 1);
		return r;
	}
	void removeLine(int lineIndex) {
		assert(lineIndex < lineCount);
		int p0Index = lineIndex * 2;
		removeIndexAt(p0Index + 1);
		removeIndexAt(p0Index);
		lineCount--;
	}

	void build() {
		gVertexBuffer::build();
		gIndexBuffer::build();
	}

};


#endif