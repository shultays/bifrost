#pragma once

#ifndef DETAILED_MAP_CONTOLLER_H__
#define DETAILED_MAP_CONTOLLER_H__

#include "gGame.h"
#include "Tools.h"
#include "Grid.h"
#include "gSlave.h"
class WorldMap;
class TerrainNode;

class NodeBuilder;

class DetailedMapController : public gTickable {
	friend class NodeBuilder;
	WorldMap* world;

	Grid<TerrainNode*> nodes;

	float cellSize;
	int cellPerNode;
	int edgePerCell;
	int squareCount;
	float cellDetail;

	WorldCoor oldCoor;
	IntVec2 oldIndex;

	IntVec2 coorToIndex(WorldCoor& coor);
	int waitingJobs;

	Vec4 cropArea;
public:
	DetailedMapController(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell);

	void updateMap(WorldCoor& coor);
	void initMap(WorldCoor& coor);

	void setCropArea(const Vec4& cropArea) {
		this->cropArea = cropArea;
	}

	void tick(float dt) override;

	Vec4 getTerrainArea() const;
};



#endif