#pragma once

#ifndef PERLIN_MAP_H__
#define PERLIN_MAP_H__

#include "gVec.h"
#include "PerlinShell.h"

#include <vector>
#include <stdlib.h>


class PerlinMap {
	std::vector<PerlinShell> shells;
	float mapSize;
public:


	PerlinMap(float mapSize = 128.0f) {
		setMapSize(mapSize);
	}

	void setMapSize(float mapSize) {
		this->mapSize = mapSize;
	}

	int getShellCount() {
		return shells.size();
	}

	void addPerlinShell(int nodeCount, float min_height, float max_height, float percentage = 0.5f, float pow_val = 1.0f, int seed = -1) {
		shells.push_back(PerlinShell(mapSize, nodeCount, min_height, max_height, percentage, pow_val, seed));
	}

	float getHeightAt(const Vec2& vec) {
		return getHeightAt(vec, shells.size());
	}
	float getHeightAt(float x, float y) {
		return getHeightAt(Vec2(x, y), shells.size());
	}
	void clear() {
		shells.clear();
	}

	float getHeightAt(const Vec2& vec, int maxShell) {
		float t = 0.0f;
		for (int i = 0; i < maxShell; i++) {
			t += shells[i].getHeightAt(vec);
		}
		return t;
	}

	float getMapSize() {
		return mapSize;
	}

};

#endif