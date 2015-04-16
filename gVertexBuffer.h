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
};

#define VERTEX_PROP_POSITION 0x1
#define VERTEX_PROP_NORMAL   0x2
#define VERTEX_PROP_COLOR    0x4
#define VERTEX_PROP_UV       0x8
#define VERTEX_PROP_TEXT_W   0x10


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

	int positionStart;
	int normalStart;
	int colorStart;
	int uvStart;
	int textureWeightsStart;

	unsigned vertexSize;
	unsigned vertexCount;


	unsigned getStartPositionAt(unsigned i) {
		return i*vertexSize;
	}
	bool built;
public:

	gVertexBuffer(bool isStatic = true) {
		built = false;
		buffer = NULL;
		vertexCount = 0;
		vertexSize = 0;
		this->isStatic = isStatic;
	}
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