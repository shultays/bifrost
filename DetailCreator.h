#pragma once

#ifndef DETAIL_CREATOR_H__
#define DETAIL_CREATOR_H__

#include "WorldMap.h"
#include "gRandom.h"

template <class T>
class DetailCreator {
protected:
	int squareCount;
	int cellPerNode;
	float cellSize;
	WorldMap* world;

	IntVec2 centerIndex;

	Grid<T*> nodes;

	std::vector<IntVec2> buildOrder;

	int waitingJobs;
	virtual void initNode(T*& node, WorldCoor& nodeCoor, gRandom& random, int a, int b) = 0;
	virtual void deleteNode(T*& node) = 0;


	IntVec2 coorToIndex(const WorldCoor& coor) {
		IntVec2 r = coor.index * cellPerNode;
		r.x += (int)(coor.pos.x / cellSize);
		r.y += (int)(coor.pos.y / cellSize);
		return r;
	}

	WorldCoor indexToCoor(const IntVec2& index) {
		WorldCoor coor;
		coor.index.x = index.x / cellPerNode;
		coor.index.y = index.y / cellPerNode;

		coor.pos.x = (index.x%cellPerNode)*cellSize;
		coor.pos.y = (index.y%cellPerNode)*cellSize;

		return coor;
	}
public:

	DetailCreator(WorldMap* world, int cellPerNode, int squareCount) {

		this->world = world;
		this->cellPerNode = cellPerNode;
		this->squareCount = squareCount;

		cellSize = world->getNodeSize() / cellPerNode;
		nodes.init(squareCount + squareCount + 1, squareCount + squareCount + 1, nullptr);

		for (int a = -squareCount; a <= squareCount; a++) {
			for (int b = -squareCount; b <= squareCount; b++) {
				buildOrder.push_back(IntVec2(a, b));
			}
		}

		std::sort(buildOrder.begin(), buildOrder.end(),
			[](IntVec2 const& a, IntVec2 const& b) { return abs(a.x) + abs(a.y) < abs(b.x) + abs(b.y); });
	}


	virtual void updateMap(WorldCoor& coor) {
		if (waitingJobs) return;

		IntVec2 newCenterIndex = coorToIndex(coor);

		const int moveCount = 2;

		if (newCenterIndex.x >= centerIndex.x + moveCount) {
			centerIndex.x += moveCount;
			WorldCoor baseCoor = indexToCoor(centerIndex);
			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a < -squareCount + moveCount) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (a <= squareCount - moveCount) {
						nodes[i][j] = nodes[i + moveCount][j];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a > squareCount - moveCount) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + centerIndex.x + a, baseCoor.index.y * cellPerNode + centerIndex.y + b);
						initNode(nodes[i][j], cellCoor, random, a, b);
					}
				}
			}
		} else if (newCenterIndex.x <= centerIndex.x - moveCount) {
			centerIndex.x -= moveCount;
			WorldCoor baseCoor = indexToCoor(centerIndex);
			for (int a = squareCount; a >= -squareCount; a--) {
				for (int b = squareCount; b >= -squareCount; b--) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a > squareCount - moveCount) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (a >= -squareCount + moveCount) {
						nodes[i][j] = nodes[i - moveCount][j];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a < -squareCount + moveCount) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + centerIndex.x + a, baseCoor.index.y * cellPerNode + centerIndex.y + b);
						initNode(nodes[i][j], cellCoor, random, a, b);
					}
				}
			}
		} else if (newCenterIndex.y >= centerIndex.y + moveCount) {
			centerIndex.y += moveCount;
			WorldCoor baseCoor = indexToCoor(centerIndex);
			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b < -squareCount + moveCount) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (b <= squareCount - moveCount) {
						nodes[i][j] = nodes[i][j + moveCount];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b > squareCount - moveCount) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + centerIndex.x + a, baseCoor.index.y * cellPerNode + centerIndex.y + b);
						initNode(nodes[i][j], cellCoor, random, a, b);
					}
				}
			}
		} else if (newCenterIndex.y <= centerIndex.y - moveCount) {
			centerIndex.y -= moveCount;
			WorldCoor baseCoor = indexToCoor(centerIndex);
			for (int a = squareCount; a >= -squareCount; a--) {
				for (int b = squareCount; b >= -squareCount; b--) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b > squareCount - moveCount) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (b >= -squareCount + moveCount) {
						nodes[i][j] = nodes[i][j - moveCount];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b < -squareCount + moveCount) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + centerIndex.x + a, baseCoor.index.y * cellPerNode + centerIndex.y + b);
						initNode(nodes[i][j], cellCoor, random, a, b);
					}
				}
			}
		}
	}

	virtual void initMap(WorldCoor& coor) {
		waitingJobs = 0;
		deleteMap();

		centerIndex = coorToIndex(coor);
		WorldCoor baseCoor = indexToCoor(centerIndex);

		for (unsigned n = 0; n < buildOrder.size(); n++) {
			int a = buildOrder[n].x;
			int b = buildOrder[n].y;
			int i = a + squareCount;
			int j = b + squareCount;
			WorldCoor cellCoor = baseCoor;
			cellCoor.pos.x += a*cellSize;
			cellCoor.pos.y += b*cellSize;
			cellCoor.fix(world->getNodeSize());

			gRandom random(baseCoor.index.x * cellPerNode + centerIndex.x + a, baseCoor.index.y * cellPerNode + centerIndex.y + b);

			initNode(nodes[i][j], cellCoor, random, a, b);
		}
	}

	virtual void deleteMap() {
		for (int i = 0; i < squareCount + squareCount + 1; i++) {
			for (int j = 0; j < squareCount + squareCount + 1; j++) {
				deleteNode(nodes[i][j]);
				nodes[i][j] = nullptr;
			}
		}
	}

	bool hasWaitingJob() {
		return waitingJobs > 0;
	}
};


#endif