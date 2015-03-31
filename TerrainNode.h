#pragma once
#ifndef TERRAIN_NODE_H__
#define TERRAIN_NODE_H__

#include "gStaticBufferedDrawables.h"
#include "Grid.h"

class WorldMap;

class TerrainNode : public gRenderable {
	gStaticIndexBufferedDrawable *drawable;

	Grid<float> heightMap;
	WorldMap *world;
public:
	TerrainNode() {
		drawable = NULL;
	}

	~TerrainNode() {
		SAFE_DELETE(drawable);
	}

	void setWorldMap(WorldMap *world) {
		this->world = world;
	}

	void build(IntVec2 index, int edgeCount);

	void render();

};


#endif