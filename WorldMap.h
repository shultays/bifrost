#pragma once

#ifndef WORLD_MAP_H__
#define WORLD_MAP_H__

#include "gVec.h"
#include "PerlinMap.h"
#include "Grid.h"

#include <vector>
#include <stdlib.h>
#include "TerrainNode.h"
#include "Tools.h"

class gIndexBufferedDrawable;
class gTexture;
class gVertexBufferRenderable;

class HeightCacher {
	int heightCacheSize;

	struct HeightCache {
		WorldCoor coor;
		float height;
	};

	HeightCache* heightCaches;
	int heightCacheIndex;
public:
	HeightCacher() {
		heightCaches = nullptr;
		heightCacheSize = -1;
	}

	void init(int size) {
		if (heightCacheSize != size) {
			SAFE_DELETE(heightCaches);
			heightCaches = new HeightCache[size];
		}
		heightCacheSize = size;
		heightCacheIndex = 0;


		heightCacheIndex = 0;
		for (int i = 0; i < heightCacheSize; i++) {
			heightCaches[i].coor.index = -1;
		}
	}

	~HeightCacher() {
		SAFE_DELETE(heightCaches);
	}

	bool getCachedHeight(const WorldCoor & coor, float& height) {
		for (int i = 0; i < heightCacheSize; i++) {
			if (heightCaches[i].coor == coor) {
				height = heightCaches[i].height;
				return true;
			}
		}
		return false;
	}

	void HeightCacher::cacheHeight(const WorldCoor & coor, float t) {
		heightCaches[heightCacheIndex].coor = coor;
		heightCaches[heightCacheIndex].height = t;
		heightCacheIndex++;
		if (heightCacheIndex >= heightCacheSize) heightCacheIndex = 0;
	}

};

class WorldMap : public gRenderable {
	friend class TerrainNode;
protected:
	PerlinMap perlinMap;
	PerlinMap detailMap;
	PerlinMap earthMap;

	float mapSize;
	float nodeSize;
	int edgeCount;

	Grid<float> heightMap;
	Grid<Vec3> normalMap;
	Grid<Vec3> colorMap;


	gIndexBufferedDrawable *terrainDrawable;
	gVertexBufferRenderable *waterDrawable;

	IntVec2 anchorPos;
	bool isScaled;

	HeightCacher mainCacher;


public:

	WorldMap(float mapSize, int edgeCount);

	~WorldMap() {
		SAFE_DELETE(terrainDrawable);
		SAFE_DELETE(waterDrawable);
		waterDrawable = NULL;
		terrainDrawable = NULL;
	}

	void build();

	float getMapSize() const {
		return mapSize;
	}
	int getEdgeCount() const {
		return edgeCount;
	}
	float getNodeSize() const {
		return nodeSize;
	}

	const IntVec2& getAnchorPos() const {
		return anchorPos;
	}

	void setAnchorPos(const IntVec2& anchorPos) {
		this->anchorPos = anchorPos;
	}
	void setIsScaled(bool isScaled) {
		this->isScaled = isScaled;
	}
	void buildHeightMap();
	void buildNormalMap();
	void buildColorMap();
	float getHeightAt(WorldCoor &coor, HeightCacher& cacher) const;
	float getHeightAt(WorldCoor &coor) {
		return getHeightAt(coor, mainCacher);
	}
	void buildBuffer();
	virtual void render() override;
};

#endif