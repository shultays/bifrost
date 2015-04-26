#pragma once

#ifndef DETAILED_MAP_CONTOLLER_H__
#define DETAILED_MAP_CONTOLLER_H__

#include "DetailCreator.h"


class DetailedMapGenerator : public DetailCreator<TerrainNode>, public gTickable {
	friend class DetailedMapNodeBuilder;

	int edgePerCell;
	float cellDetail;

	Vec4 cropArea;
public:
	DetailedMapGenerator(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell);


	void setCropArea(const Vec4& cropArea) {
		this->cropArea = cropArea;
	}

	void tick(float dt) override;

	Vec4 getTerrainArea() const;


	virtual void initNode(TerrainNode*& node, WorldCoor& nodeCoor, gRandom& random, int a, int b);
	virtual void deleteNode(TerrainNode*& node);
};



#endif