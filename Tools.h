#pragma once
#ifndef TOOLS_H__
#define TOOLS_H__

#include "gVec.h"


class WorldCoor {
public:
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


	IntVec2 index;
	Vec2 pos;
	float h;
};

#endif