#pragma once

#ifndef G_VERTEX_BUFFER_H__
#define G_VERTEX_BUFFER_H__

#include "gTools.h"

class VertexPointer {
public:
	Vec3* position;
	Vec3* normal;
	Vec4* color;
	Vec2* uv;
	Vec4* textureWeights;
	void* extra;
};

#define VERTEX_PROP_POSITION 0x100
#define VERTEX_PROP_NORMAL   0x200
#define VERTEX_PROP_COLOR    0x400
#define VERTEX_PROP_UV       0x800
#define VERTEX_PROP_TEXT_W   0x1000


class gVertexBuffer {
	GLuint gl_buffer;
	byte *buffer;
protected:
	bool isStatic;

	bool hasPosition;
	bool hasNormal;
	bool hasColor;
	bool hasUV;
	bool hasTextureWeights;

	bool hasConstantNormal;
	Vec3 constantNormal;
	bool hasConstantColor;
	Vec4 constantColor;
	bool hasConstantTextureWeights;
	Vec4 constantTextureWeights;

	bool hasExtra;
	int extraSize;

	int positionStart;
	int normalStart;
	int colorStart;
	int uvStart;
	int textureWeightsStart;
	int extraStart;

	unsigned vertexCount;
	unsigned maxVertexCount;

	unsigned getStartPositionAt(unsigned i) {
		return i*vertexCount;
	}
	bool built;
public:

	gVertexBuffer(int props, unsigned vertexCount, bool isStatic = true);
	void setConstantNormal(const Vec3& normal);
	void setConstantColor(const Vec4& color);
	void setConstantTextureWeights(const Vec4& textureWeights);
	~gVertexBuffer();

	VertexPointer getVertexPointerAt(unsigned i);
	void build();
	void bind();
};


#endif