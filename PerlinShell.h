#pragma once

#ifndef PERLIN_SHELL_H__
#define PERLIN_SHELL_H__

#include "gVec.h"
#include <functional>
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
	float getNodeHeight(int i, int j) {
		return ((unsigned int)(hasher(i + edgeCount*j)) > acceptHash) ? maxHeight : minHeight;
	}

public:
	PerlinShell(float mapSize, int edgeCount, float minHeight, float maxHeight, float percentage = 0.5f, unsigned int seed = -1) {
		this->cellSize = mapSize / (edgeCount - 1);
		if (seed == -1) {
			seed = rand();
		}
		this->seed = seed;
		this->edgeCount = edgeCount;
		this->maxHeight = maxHeight;
		this->minHeight = minHeight;
		this->diffHeight = maxHeight - minHeight;
		acceptHash = (unsigned int)(maxHash * percentage);

		rotate.makeIdentity();
		rotate.rotateBy((rand() % 1000) / 1000.0f);
		add = Vec2(cellSize*((rand() % 1000) / 1000.0f), cellSize*((rand() % 1000) / 1000.0f));
	}
	float getHeightAt(const Vec2& inputPos) {
		Vec2 pos = inputPos*rotate;
		pos += add;
		int i, j;
		float dx, dy;
		float tx = pos.x / cellSize;
		float ty = pos.y / cellSize;
		i = (int)floor(tx);
		j = (int)floor(ty);
		dx = tx - i;
		dy = ty - j;

		// p2 -- p4
		// |  \  |
		// p0 -- p1

		if (dx + dy < 1.0f) {
			float p0 = getNodeHeight(i, j);
			float p1 = getNodeHeight(i + 1, j);
			float p2 = getNodeHeight(i, j + 1);


			return p0 * (1.0f - dx - dy) + dx * p1 + dy * p2;
		} else {
			float p4 = getNodeHeight(i + 1, j + 1);
			float p1 = getNodeHeight(i + 1, j);
			float p2 = getNodeHeight(i, j + 1);

			dx = 1.0f - dx;
			dy = 1.0f - dy;

			return p4 * (1.0f - dx - dy) + dx * p2 + dy * p1;
		}
	}
};

#endif