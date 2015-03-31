#pragma once
#ifndef TOOLS_H__
#define TOOLS_H__

#include "gVec.h"


class WorldCoor {
public:
	WorldCoor() {
		index.setZero();
		pos.setZero();
	}

	WorldCoor(int i, int j) {
		index.x = i;
		index.y = j;
		pos.setZero();
	}

	WorldCoor(int i, int j, float x, float y) {
		index.x = i;
		index.y = j;
		pos.x = x;
		pos.y = y;
	}

	WorldCoor(const IntVec2& index, const Vec2& pos) {
		this->index = index;
		this->pos = pos;
	}

	IntVec2 index;
	Vec2 pos;
	float h;

	void fix(float maxSize) {
		while (pos.x >= maxSize) {
			pos.x -= maxSize;
			index.x++;
		}
		while (pos.x < 0.0f) {
			pos.x += maxSize;
			index.x--;
		}
		while (pos.y >= maxSize) {
			pos.y -= maxSize;
			index.y++;
		}
		while (pos.y < 0.0f) {
			pos.y += maxSize;
			index.y--;
		}
	}
};

#endif