#pragma once

#ifndef SKY_H__
#define SKY_H__

#include "gIndexBufferedDrawable.h"
#include "Tools.h"
#include "gRandom.h"
class WorldMap;

class Sky {
	gVertexBufferRenderable* renderable;
	WorldMap* world;
	gRandom skyRandom;
public:
	Sky(WorldMap* world);
	~Sky();
	void updateSky(WorldCoor& coor);
	void initSky(WorldCoor& coor);
	void deleteSky();
};


#endif