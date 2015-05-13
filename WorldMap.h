#pragma once

#ifndef WORLD_MAP_H__
#define WORLD_MAP_H__

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "gVec.h"
#include "PerlinMap.h"
#include "Grid.h"

#include "TerrainNode.h"
#include "Tools.h"

class gIndexBufferedDrawable;
class gTexture;
class gVertexBufferRenderable;

struct DrainageNode {
	WorldCoor coor;
	IntVec2 drainageIndex;
	Vec3 normal;
	float drainage;
	bool hasWater;
	int waterIndex;
};

class HeightCacher {
	int heightCacheSize;

	struct HeightCache {
		WorldCoor coor;
		float height;
	};

	HeightCache* heightCaches;
	int heightCacheIndex;
public:
	HeightCacher() {
		heightCaches = nullptr;
		heightCacheSize = -1;
	}

	void init(int size) {
		if (heightCacheSize != size) {
			SAFE_DELETE(heightCaches);
			heightCaches = new HeightCache[size];
		}
		heightCacheSize = size;
		heightCacheIndex = 0;

		clear();
	}

	void clear() {
		heightCacheIndex = 0;
		for (int i = 0; i < heightCacheSize; i++) {
			heightCaches[i].coor.index = -1;
		}
	}

	~HeightCacher() {
		SAFE_DELETE(heightCaches);
	}

	bool getCachedHeight(const WorldCoor & coor, float& height) {
		if (!heightCaches) return false;
		for (int i = 0; i < heightCacheSize; i++) {
			if (heightCaches[i].coor == coor) {
				height = heightCaches[i].height;
				return true;
			}
		}
		return false;
	}

	void HeightCacher::cacheHeight(const WorldCoor & coor, float t) {
		if (!heightCaches) return;
		heightCaches[heightCacheIndex].coor = coor;
		heightCaches[heightCacheIndex].height = t;
		heightCacheIndex++;
		if (heightCacheIndex >= heightCacheSize) heightCacheIndex = 0;
	}
};

class WorldMap : public gRenderable {
	friend class TerrainNode;
protected:
	PerlinMap perlinMap;
	PerlinMap detailMap;
	PerlinMap earthMap;

	float mapSize;
	float nodeSize;
	int edgeCount;


	Grid<float> heightMap;
	Grid<Vec3> normalMap;
	Grid<Vec3> colorMap;
	Grid<int> islandMap;


	gVertexBufferRenderable *terrainDrawable;
	gVertexBufferRenderable *waterDrawable;

	IntVec2 anchorPos;
	bool isScaled;

	HeightCacher mainCacher;

	int drainageEdgeCount;
	float maxDrainage, minDrainage;
	std::vector<DrainageNode*> drainageNodes;
	std::vector<DrainageNode*> nearWaterNodes;
	Grid<float> distanceToWater;
	Grid<DrainageNode> drainageGrid;


public:

	WorldMap(float mapSize, int edgeCount);

	~WorldMap() {
		SAFE_DELETE(terrainDrawable);
		SAFE_DELETE(waterDrawable);
		waterDrawable = NULL;
		terrainDrawable = NULL;
	}

	void build();

	float getMapSize() const {
		return mapSize;
	}
	int getEdgeCount() const {
		return edgeCount;
	}
	float getNodeSize() const {
		return nodeSize;
	}

	const IntVec2& getAnchorPos() const {
		return anchorPos;
	}

	void setAnchorPos(const IntVec2& anchorPos) {
		this->anchorPos = anchorPos;
	}
	void setIsScaled(bool isScaled) {
		this->isScaled = isScaled;
	}
	void buildHeightMap();
	void buildNormalMap();
	void buildColorMap();
	float getHeightAt(WorldCoor &coor, HeightCacher& cacher) const;
	float getHeightAt(WorldCoor &coor) {
		return getHeightAt(coor, mainCacher);
	}
	void buildBuffer();
	virtual void render() override;
	Vec3 toGamePos(WorldCoor &coor);
	Vec3 toGamePos(WorldCoor &coor, HeightCacher& cacher) const;
	float getTreeProbabilityAt(WorldCoor &coor, HeightCacher& cacher) const;
	void buildDrainage();
	void buildRivers();
	bool generateRiver(const IntVec2& start_index, const IntVec2& end_index);
	Vec3 drainageNodeToVec3(const DrainageNode& node);
	Vec3 toWorldGamePos(int i, int j);



	void serialize(gBinaryFileOutputStream& output) {
		perlinMap.serialize(output);
		detailMap.serialize(output);
		earthMap.serialize(output);

		output << mapSize;
		output << nodeSize;
		output << edgeCount;

		heightMap.serialize(output);
		normalMap.serialize(output);
		colorMap.serialize(output);
		islandMap.serialize(output);

		output << drainageEdgeCount;
		output << maxDrainage;
		output << minDrainage;

		distanceToWater.serialize(output);
		drainageGrid.serialize(output);

		output << (int)drainageNodes.size();
		for (unsigned i = 0; i < drainageNodes.size(); i++) {
			output << (int)(drainageNodes[i] - &drainageGrid[0][0]);
		}

		output << (int)nearWaterNodes.size();
		for (unsigned i = 0; i < nearWaterNodes.size(); i++) {
			output << (int)(nearWaterNodes[i] - &nearWaterNodes[0][0]);
		}
	}

	void deserialize(gBinaryFileInputStream& input) {
		mainCacher.clear();

		perlinMap.deserialize(input);
		detailMap.deserialize(input);
		earthMap.deserialize(input);

		input >> mapSize;
		input >> nodeSize;
		input >> edgeCount;

		heightMap.deserialize(input);
		normalMap.deserialize(input);
		colorMap.deserialize(input);
		islandMap.deserialize(input);

		input >> drainageEdgeCount;
		input >> maxDrainage;
		input >> minDrainage;

		distanceToWater.deserialize(input);
		drainageGrid.deserialize(input);

		int size;

		input >> size;
		drainageNodes.clear();
		for (int i = 0; i < size; i++) {
			int index;
			input >> index;
			drainageNodes.push_back(&drainageGrid.atRawIndex(index));
		}

		input >> size;
		nearWaterNodes.clear();
		for (int i = 0; i < size; i++) {
			int index;
			input >> index;
			nearWaterNodes.push_back(&drainageGrid.atRawIndex(index));
		}
	}

};

#endif