#pragma once
#ifndef TOOLS_H__
#define TOOLS_H__

#include "gVec.h"

#define INVALID_HEIGHT -1000000.0f

class WorldCoor {
public:
	WorldCoor() {
		index.setZero();
		pos.setZero();
		h = INVALID_HEIGHT;
	}

	WorldCoor(int i, int j) {
		index.x = i;
		index.y = j;
		pos.setZero();
		h = INVALID_HEIGHT;
	}

	WorldCoor(int i, int j, float x, float y) {
		index.x = i;
		index.y = j;
		pos.x = x;
		pos.y = y;
		h = INVALID_HEIGHT;
	}

	WorldCoor(const IntVec2& index, const Vec2& pos) {
		this->index = index;
		this->pos = pos;
		h = INVALID_HEIGHT;
	}

	WorldCoor(const WorldCoor& coor, int i, int j, float x, float y) {
		*this = coor;
		index.x += i;
		index.y += j;
		pos.x += x;
		pos.y += y;
		this->h = coor.h;
	}

	WorldCoor operator-(const WorldCoor &other) const {
		WorldCoor r = *this;
		r.index -= other.index;
		r.pos -= other.pos;
		return r;
	}


	bool operator==(const WorldCoor &other) const {
		return index == other.index && pos.almostEquals(other.pos);
	}

	bool operator!=(const WorldCoor &other) const {
		return !(*this == other);
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