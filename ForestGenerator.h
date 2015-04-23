#pragma once


#ifndef FOREST_GENERATOR_H__
#define FOREST_GENERATOR_H__

#include "gTools.h"
#include "Tools.h"
#include "Grid.h"


class WorldMap;
class gRenderableGroup;

class ForestGenerator {
	int squareCount;
	int cellPerNode;
	float cellSize;

	WorldMap* world;

	Grid<gRenderableGroup*> nodes;
public:
	ForestGenerator(WorldMap* world);

	void updateMap(WorldCoor& coor);
	void initMap(WorldCoor& coor);


};


#endif