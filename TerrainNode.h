#pragma once
#ifndef TERRAIN_NODE_H__
#define TERRAIN_NODE_H__

#include "gIndexBufferedDrawable.h"
#include "Grid.h"
#include "Tools.h"

class WorldMap;

class TerrainNode : public gRenderable {
	gVertexBufferRenderable *drawable;

	WorldMap *world;
	bool isBuilt;

	Vec2 startPoint;
	Vec2 endPoint;

	Vec4 cropArea;
public:
	TerrainNode(WorldMap *world);

	const Vec2& getMin() const {
		return startPoint;
	}
	const Vec2& getMax() const {
		return endPoint;
	}

	~TerrainNode() {
		SAFE_DELETE(drawable);
	}


	void build(WorldCoor start, Vec2 size, int edgeCount);
	void buildMesh();

	void render();

	bool getIsBuilt() {
		return isBuilt;
	}
	void setDiscardArea(const Vec4& cropArea) {
		this->cropArea = cropArea;
	}
};


#endif