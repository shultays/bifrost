#include "gStaticVertexBuffer.h"

#include "gGlobals.h"
#include "gGame.h"
#include "gShader.h"

gStaticVertexBuffer::gStaticVertexBuffer(int props, unsigned vertexCount) {
	hasPosition = (props & VERTEX_PROP_POSITION) > 0;
	hasNormal = (props & VERTEX_PROP_NORMAL) > 0;
	hasColor = (props & VERTEX_PROP_COLOR) > 0;
	hasUV = (props & VERTEX_PROP_UV) > 0;
	hasTextureWeights = (props & VERTEX_PROP_TEXT_W) > 0;

	vertexSize = 0;
	positionStart = normalStart = colorStart = uvStart = textureWeightsStart = -1;

	if (hasPosition) {
		positionStart = vertexSize;
		vertexSize += sizeof(Vec3);
	}

	if (hasNormal) {
		normalStart = vertexSize;
		vertexSize += sizeof(Vec3);
	}
	if (hasColor) {
		colorStart = vertexSize;
		vertexSize += sizeof(Vec4);
	}
	if (hasUV) {
		uvStart = vertexSize;
		vertexSize += sizeof(Vec2);
	}
	if (hasTextureWeights) {
		textureWeightsStart = vertexSize;
		vertexSize += sizeof(Vec4);
	}

	this->vertexCount = vertexCount;
	int buffsize = getStartPositionAt(vertexCount);
	buffer = new byte[buffsize];
	built = false;
}

void gStaticVertexBuffer::setConstantNormal(const Vec3& normal) {
	assert(!hasNormal);
	hasConstantNormal = true;
	constantNormal = normal;
}

void gStaticVertexBuffer::setConstantColor(const Vec4& color) {
	assert(!hasColor);
	hasConstantColor = true;
	constantColor = color;
}

void gStaticVertexBuffer::setConstantTextureWeights(const Vec4& textureWeights) {
	assert(!hasTextureWeights);
	hasConstantTextureWeights = true;
	constantTextureWeights = textureWeights;
}

gStaticVertexBuffer::~gStaticVertexBuffer() {
	SAFE_DELETE_ARR(buffer);
	if (built) {
		glDeleteBuffers(1, &gl_buffer);
		built = false;
	}
}

VertexPointer gStaticVertexBuffer::getVertexPointerAt(unsigned i) {
	assert(!built);
	assert(buffer);
	assert(i >= 0 && i < vertexCount);
	VertexPointer pointer;
	byte* start = buffer + getStartPositionAt(i);

	if (hasPosition) {
		pointer.position = (Vec3*)(start + positionStart);
	} else {
		pointer.position = NULL;
	}

	if (hasNormal) {
		pointer.normal = (Vec3*)(start + normalStart);
	} else if (hasConstantNormal) {
		pointer.normal = &constantNormal;
	} else {
		pointer.normal = NULL;
	}

	if (hasColor) {
		pointer.color = (Vec4*)(start + colorStart);
	} else if (hasConstantColor) {
		pointer.color = &constantColor;
	} else {
		pointer.color = NULL;
	}

	if (hasUV) {
		pointer.uv = (Vec2*)(start + uvStart);
	} else {
		pointer.uv = NULL;
	}

	if (hasTextureWeights) {
		pointer.textureWeights = (Vec4*)(start + textureWeightsStart);
	} else if (hasConstantColor) {
		pointer.textureWeights = &constantTextureWeights;
	} else {
		pointer.textureWeights = NULL;
	}
	return pointer;
}

void gStaticVertexBuffer::build() {
	assert(!built);
	assert(buffer);
	built = true;
	glGenBuffers(1, &gl_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, gl_buffer);
	glBufferData(GL_ARRAY_BUFFER, getStartPositionAt(vertexCount), buffer, GL_STATIC_DRAW);

	SAFE_DELETE_ARR(buffer);
}

void gStaticVertexBuffer::bind() {
	assert(built);
	glBindBuffer(GL_ARRAY_BUFFER, gl_buffer);

	if (hasPosition) {
		gears.game->currentShader->bindPosition(vertexSize, positionStart);
	}
	if (hasNormal) {
		gears.game->currentShader->bindNormal(vertexSize, normalStart);
	} else if (hasConstantNormal) {
		gears.game->currentShader->setAttributeNormal(constantNormal);
	}

	if (hasColor) {
		gears.game->currentShader->bindColor(vertexSize, colorStart);
	} else if (hasConstantColor) {
		gears.game->currentShader->setAttributeColor(constantColor);
	}

	if (hasUV) {
		gears.game->currentShader->bindUV(vertexSize, uvStart);
	}
	if (hasTextureWeights) {
		gears.game->currentShader->bindPosition(vertexSize, textureWeightsStart);
	} else if (hasConstantTextureWeights) {
		gears.game->currentShader->setAttributeTextureWeights(constantTextureWeights);
	}

}
