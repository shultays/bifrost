#pragma once

#ifndef PERLIN_SHELL_H__
#define PERLIN_SHELL_H__

#include "gVec.h"
#include "gMat.h"
#include "Tools.h"
#include "functional"

class WorldMap;

class PerlinShell {
	int edgeCount;
	unsigned int seed;
	float cellSize;
	std::hash<int> hasher;
	float minHeight, maxHeight, diffHeight;
	Mat2 rotate;
	Vec2 add;
	const unsigned int maxHash = std::numeric_limits<size_t>::max();
	unsigned int acceptHash;
	WorldMap *world;
	float pow_val;
	int pow_type;
	float cellPerNode;

	enum {
		pow_1 = 1,
		pow_2 = 2,
		pow_0_5 = -1,
		pow_n = -2,
	};

	float getNodeHeight(int i, int j);

public:
	PerlinShell(WorldMap* world, int edgeCount, float minHeight, float maxHeight, float percentage = 0.5f, float pow_val = 1.0f, unsigned int seed = -1);
	float getHeightAt(const WorldCoor& coor);
};

#endif