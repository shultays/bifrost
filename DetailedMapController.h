#pragma once

#ifndef DETAILED_MAP_CONTOLLER_H__
#define DETAILED_MAP_CONTOLLER_H__

#include "DetailCreator.h"


class DetailedMapController : public DetailCreator<TerrainNode>, public gTickable {
	friend class NodeBuilder;

	int edgePerCell;
	float cellDetail;

	Vec4 cropArea;
public:
	DetailedMapController(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell);


	void setCropArea(const Vec4& cropArea) {
		this->cropArea = cropArea;
	}

	void tick(float dt) override;

	Vec4 getTerrainArea() const;


	virtual void initNode(TerrainNode*& node, WorldCoor& nodeCoor, gRandom& random);
	virtual void deleteNode(TerrainNode*& node);
};



#endif