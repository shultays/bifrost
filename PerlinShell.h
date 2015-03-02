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

	float pow_val;
	int pow_type;

	enum {
		pow_1 = 1,
		pow_2 = 2,
		pow_0_5 = -1,
		pow_n = -2,
	};

	float getNodeHeight(int i, int j) {
		return ((unsigned int)(hasher(i + edgeCount*j)) > acceptHash) ? 1.0f : 0.0f;
	}

public:
	PerlinShell(float mapSize, int edgeCount, float minHeight, float maxHeight, float percentage = 0.5f, float pow_val = 1.0f, unsigned int seed = -1) {
		this->cellSize = mapSize / (edgeCount - 1);
		if (seed == -1) {
			seed = rand();
		}
		this->seed = seed;
		this->edgeCount = edgeCount;
		this->maxHeight = maxHeight;
		this->minHeight = minHeight;
		this->diffHeight = maxHeight - minHeight;
		this->pow_val = pow_val;
		if (almost_equal(pow_val, 1.0f)) {
			pow_type = pow_1;
		} else if (almost_equal(pow_val, 2.0f)) {
			pow_type = pow_2;
		} else if (almost_equal(pow_val, 0.5f)) {
			pow_type = pow_0_5;
		} else if (almost_equal(pow_val, roundf(pow_val))) {
			pow_type = (int)roundf(pow_val);
		} else {
			pow_type = pow_n;
		}
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

		float r;

		if (dx + dy < 1.0f) {
			float p0 = getNodeHeight(i, j);
			float p1 = getNodeHeight(i + 1, j);
			float p2 = getNodeHeight(i, j + 1);


			r = p0 * (1.0f - dx - dy) + dx * p1 + dy * p2;
		} else {
			float p4 = getNodeHeight(i + 1, j + 1);
			float p1 = getNodeHeight(i + 1, j);
			float p2 = getNodeHeight(i, j + 1);

			dx = 1.0f - dx;
			dy = 1.0f - dy;

			r = p4 * (1.0f - dx - dy) + dx * p2 + dy * p1;
		}

		switch (pow_type) {
			case pow_1:
				return r * diffHeight + minHeight;
			case pow_2:
				return r*r * diffHeight + minHeight;
			case pow_0_5:
				return sqrt(r) * diffHeight + minHeight;
			case pow_n:
				return pow(r, pow_val) * diffHeight + minHeight;
			default:
			{
					   float r2 = r;
					   for (int i = 1; i < pow_type; i++) r2 *= r;
					   return r2 * diffHeight + minHeight;
			}
		}
	}
};

#endif