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

#define WORLD_MAP_SCALE 0.001f


class gStaticIndexBufferedDrawable;
class gTexture;
class gStaticTriangleVertexBufferDrawable;


class WorldMap : public gRenderable {
	PerlinMap perlinMap;
	PerlinMap detailMap;
	PerlinMap earthMap;

	float mapSize;
	float nodeSize;
	int edgeCount;

	Grid<float> heightMap;
	Grid<Vec3> normalMap;
	Grid<Vec3> colorMap;

	gTexture* texture;

	gStaticIndexBufferedDrawable *terrainDrawable;
	gStaticTriangleVertexBufferDrawable *waterDrawable;
	TerrainNode node;

public:
	WorldMap() : WorldMap(512 * 1024.0f, 128) {}

	WorldMap(float mapSize, int edgeCount);

	~WorldMap() {
		SAFE_DELETE(terrainDrawable);
		SAFE_DELETE(waterDrawable);
		waterDrawable = NULL;
		terrainDrawable = NULL;
	}

	void build();
	float getMapSize();
	float getNodeSize();
	void buildHeightMap();
	void buildNormalMap();
	void buildColorMap();
	float getHeightAt(WorldCoor &coor);
	void buildBuffer();
	virtual void render() override;
};

#endif