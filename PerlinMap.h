#pragma once

#ifndef PERLIN_MAP_H__
#define PERLIN_MAP_H__

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "gVec.h"
#include "PerlinShell.h"
#include "Tools.h"
#include "gBinaryStream.h"

class PerlinMap {
	std::vector<PerlinShell> shells;
	WorldMap *world;
public:
	PerlinMap() {}

	void setWorldMap(WorldMap *world) {
		this->world = world;
	}

	int getShellCount() {
		return shells.size();
	}

	void addPerlinShell(int nodeCount, float min_height, float max_height, float percentage = 0.5f, float pow_val = 1.0f, int seed = -1) {
		shells.push_back(PerlinShell(world, nodeCount, min_height, max_height, percentage, pow_val, seed));
	}

	float getHeightAt(const WorldCoor& coor) const {
		return getHeightAt(coor, shells.size());
	}

	float getHeightAt(const WorldCoor& coor, int maxShell) const {
		float t = 0.0f;
		for (int i = 0; i < maxShell; i++) {
			t += shells.at(i).getHeightAt(coor);
		}
		return t;
	}

	void clear() {
		shells.clear();
	}

	WorldMap* getWorldMap() {
		return world;
	}

	void serialize(gBinaryFileOutputStream& output) {
		output << shells.size();
		for (unsigned i = 0; i < shells.size(); i++) {
			shells.at(i).serialize(output);
		}
	}
	void deserialize(gBinaryFileInputStream& input) {
		int size;
		input >> size;
		shells.clear();
		shells.resize(size);
		for (unsigned i = 0; i < shells.size(); i++) {
			shells.at(i).deserialize(input);
			shells.at(i).setWorldMap(world);
		}
	}
};

#endif