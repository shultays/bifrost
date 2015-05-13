#pragma once

#ifndef PERLIN_SHELL_H__
#define PERLIN_SHELL_H__

#include <iostream>
#include "gVec.h"
#include "gMat.h"
#include "Tools.h"
#include "gBinaryStream.h"
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
	unsigned int maxHash;
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

	float getNodeHeight(int i, int j) const;

public:
	PerlinShell() {}

	void setWorldMap(WorldMap* world) {
		this->world = world;
	}

	PerlinShell(WorldMap* world, int edgeCount, float minHeight, float maxHeight, float percentage = 0.5f, float pow_val = 1.0f, unsigned int seed = -1);
	float getHeightAt(const WorldCoor& coor) const;


	void serialize(gBinaryFileOutputStream& output) {
		output << edgeCount;
		output << seed;
		output << cellSize;
		output << minHeight;
		output << maxHeight;
		output << diffHeight;

		output << rotate;
		output << add;

		output << maxHash;
		output << acceptHash;
		output << pow_val;
		output << pow_type;
		output << cellPerNode;
	}
	void deserialize(gBinaryFileInputStream& input) {
		input >> edgeCount;
		input >> seed;
		input >> cellSize;
		input >> minHeight;
		input >> maxHeight;
		input >> diffHeight;

		input >> rotate;
		input >> add;

		input >> maxHash;
		input >> acceptHash;
		input >> pow_val;
		input >> pow_type;
		input >> cellPerNode;
	}
};

#endif