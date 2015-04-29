#include "gVertexBuffer.h"

#include "gGlobals.h"
#include "gGame.h"
#include "gShader.h"

gVertexBuffer::gVertexBuffer() {
	inited = false;
	built = false;
	buffer = nullptr;
}

gVertexBuffer::gVertexBuffer(int props, unsigned maxVertexCount, bool isStatic) {
	inited = false;
	built = false;
	buffer = nullptr;
	init(props, maxVertexCount, isStatic);
}


void gVertexBuffer::setConstantNormal(const Vec3& normal) {
	assert(!hasNormal);
	hasConstantNormal = true;
	constantNormal = normal;
}

void gVertexBuffer::setConstantColor(const Vec4& color) {
	assert(!hasColor);
	hasConstantColor = true;
	constantColor = color;
}

void gVertexBuffer::setConstantTextureWeights(const Vec4& textureWeights) {
	assert(!hasTextureWeights);
	hasConstantTextureWeights = true;
	constantTextureWeights = textureWeights;
}

gVertexBuffer::~gVertexBuffer() {
	SAFE_DELETE_ARR(buffer);
	if (built) {
		glDeleteBuffers(1, &gl_buffer);
		built = false;
	}
}

VertexPointer gVertexBuffer::getVertexPointerAt(unsigned i) {
	assert(inited);
	assert(!built || !isStatic);
	assert(buffer);
	assert(i >= 0 && i < maxVertexCount);
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

	if (hasExtra) {
		pointer.extra = (void*)(start + extraStart);
	}

	return pointer;
}

void gVertexBuffer::build() {
	assert(inited);
	assert(!built || !isStatic);
	assert(buffer);

	if (!built) {
		built = true;
		glGenBuffers(1, &gl_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer);
		glBufferData(GL_ARRAY_BUFFER, getStartPositionAt(maxVertexCount), buffer, isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, getStartPositionAt(maxVertexCount), buffer);
	}

	if (isStatic) {
		SAFE_DELETE_ARR(buffer);
	}
}

void gVertexBuffer::bind() {
	assert(inited);
	assert(built || !isStatic);
	if (built) {
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
}

void gVertexBuffer::init(int props, unsigned maxVertexCount, bool isStatic) {
	assert(inited == false);
	inited = true;
	this->isStatic = isStatic;
	hasPosition = (props & VERTEX_PROP_POSITION) > 0;
	hasNormal = (props & VERTEX_PROP_NORMAL) > 0;
	hasColor = (props & VERTEX_PROP_COLOR) > 0;
	hasUV = (props & VERTEX_PROP_UV) > 0;
	hasTextureWeights = (props & VERTEX_PROP_TEXT_W) > 0;

	this->extraSize = props & 0xFF;
	hasExtra = extraSize > 0;

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
	if (hasExtra) {
		extraStart = vertexSize;
		vertexSize += extraSize;
	}
	this->maxVertexCount = maxVertexCount;
	int buffsize = getStartPositionAt(maxVertexCount);
	buffer = new byte[buffsize];
	built = false;
}
