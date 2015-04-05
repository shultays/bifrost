#pragma once
#ifndef TERRAIN_NODE_H__
#define TERRAIN_NODE_H__

#include "gStaticBufferedDrawables.h"
#include "Grid.h"
#include "Tools.h"

class WorldMap;

class TerrainNode : public gRenderable {
	gStaticIndexBufferedDrawable *drawable;

	WorldMap *world;
public:
	TerrainNode(WorldMap *world) {
		drawable = NULL;
		this->world = world;
	}

	~TerrainNode() {
		SAFE_DELETE(drawable);
	}


	void build(WorldCoor start, Vec2 size, int edgeCount);

	void render();

};


#endif