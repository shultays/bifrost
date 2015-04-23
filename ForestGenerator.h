#pragma once


#ifndef FOREST_GENERATOR_H__
#define FOREST_GENERATOR_H__

#include "gTools.h"
#include "Tools.h"
#include "Grid.h"
#include <vector>
#include "DetailCreator.h"
class WorldMap;
class gVertexBufferRenderable;

class ForestGenerator : public DetailCreator<gVertexBufferRenderable> {
	friend class ForestNodeBuilder;

public:
	ForestGenerator(WorldMap* world);

	virtual void initNode(gVertexBufferRenderable*& node, WorldCoor& nodeCoor, gRandom& random) override;
	virtual void deleteNode(gVertexBufferRenderable*& node) override;
};


#endif