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

class DetailedMapController {
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
public:
	DetailedMapController(WorldMap* world);


	void updateMap(WorldCoor& coor);
	void initMap(WorldCoor& coor);
};


class NodeBuilder : public gSlaveWork {
public:
	DetailedMapController* mapController;
	TerrainNode* node;
	WorldCoor cellCoor;
	float cellSize;
	int edgePerCell;

	NodeBuilder(DetailedMapController* mapController, TerrainNode* node, WorldCoor cellCoor, float cellSize, int edgePerCell) {
		this->mapController = mapController;
		this->node = node;
		this->cellCoor = cellCoor;
		this->cellSize = cellSize;
		this->edgePerCell = edgePerCell;
	}
	virtual void runOnSlave();
	virtual void runOnMain();
};

#endif