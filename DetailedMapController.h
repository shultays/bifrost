#pragma once

#ifndef DETAILED_MAP_CONTOLLER_H__
#define DETAILED_MAP_CONTOLLER_H__

#include "gGame.h"
#include "Tools.h"
#include "Grid.h"

class WorldMap;
class TerrainNode;


class DetailedMapController {
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
public:
	DetailedMapController(WorldMap* world);


	void updateMap(WorldCoor& coor);
	void initMap(WorldCoor& coor);
};


#endif