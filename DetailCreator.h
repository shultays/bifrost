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

	WorldCoor oldCoor;

	Grid<T*> nodes;

	std::vector<IntVec2> buildOrder;

	int waitingJobs;
	virtual void initNode(T*& node, WorldCoor& nodeCoor, gRandom& random) = 0;
	virtual void deleteNode(T*& node) = 0;
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
		WorldCoor baseCoor = coor;
		int baseX = ((int)(baseCoor.pos.x / cellSize));
		int baseY = ((int)(baseCoor.pos.y / cellSize));
		baseCoor.pos.x = baseX * cellSize;
		baseCoor.pos.y = baseY * cellSize;

		WorldCoor diff = baseCoor - oldCoor;
		Vec2 diffPos = diff.pos + diff.index.toVec() * world->getNodeSize();

		if (diffPos.x > cellSize) {
			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a < -squareCount + 2) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (a <= squareCount - 2) {
						nodes[i][j] = nodes[i + 2][j];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a > squareCount - 2) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + baseX + a, baseCoor.index.y * cellPerNode + baseY + b);
						initNode(nodes[i][j], cellCoor, random);
					}
				}
			}
			oldCoor.index.x = baseCoor.index.x;
			oldCoor.pos.x = baseCoor.pos.x;
		}


		if (diffPos.x < -cellSize) {
			for (int a = squareCount; a >= -squareCount; a--) {
				for (int b = squareCount; b >= -squareCount; b--) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a > squareCount - 2) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (a >= -squareCount + 2) {
						nodes[i][j] = nodes[i - 2][j];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (a < -squareCount + 2) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + baseX + a, baseCoor.index.y * cellPerNode + baseY + b);
						initNode(nodes[i][j], cellCoor, random);
					}
				}
			}

			oldCoor.index.x = baseCoor.index.x;
			oldCoor.pos.x = baseCoor.pos.x;
		}


		if (diffPos.y > cellSize) {
			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b < -squareCount + 2) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (b <= squareCount - 2) {
						nodes[i][j] = nodes[i][j + 2];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b > squareCount - 2) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + baseX + a, baseCoor.index.y * cellPerNode + baseY + b);
						initNode(nodes[i][j], cellCoor, random);
					}
				}
			}
			oldCoor.index.y = baseCoor.index.y;
			oldCoor.pos.y = baseCoor.pos.y;
		}


		if (diffPos.y < -cellSize) {
			for (int a = squareCount; a >= -squareCount; a--) {
				for (int b = squareCount; b >= -squareCount; b--) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b > squareCount - 2) {
						deleteNode(nodes[i][j]);
						nodes[i][j] = nullptr;
					}
					if (b >= -squareCount + 2) {
						nodes[i][j] = nodes[i][j - 2];
					}
				}
			}

			for (int a = -squareCount; a <= squareCount; a++) {
				for (int b = -squareCount; b <= squareCount; b++) {
					int i = a + squareCount;
					int j = b + squareCount;

					if (b < -squareCount + 2) {
						WorldCoor cellCoor = baseCoor;
						cellCoor.pos.x += a*cellSize;
						cellCoor.pos.y += b*cellSize;
						cellCoor.fix(world->getNodeSize());

						gRandom random(baseCoor.index.x * cellPerNode + baseX + a, baseCoor.index.y * cellPerNode + baseY + b);
						initNode(nodes[i][j], cellCoor, random);
					}
				}
			}

			oldCoor.index.y = baseCoor.index.y;
			oldCoor.pos.y = baseCoor.pos.y;
		}
	}

	virtual void initMap(WorldCoor& coor) {
		waitingJobs = 0;
		deleteMap();

		IntVec3 centerIndex;

		WorldCoor baseCoor = coor;
		int baseX = ((int)(baseCoor.pos.x / cellSize));
		int baseY = ((int)(baseCoor.pos.y / cellSize));
		baseCoor.pos.x = baseX * cellSize;
		baseCoor.pos.y = baseY * cellSize;

		oldCoor = baseCoor;

		for (unsigned n = 0; n < buildOrder.size(); n++) {
			int a = buildOrder[n].x;
			int b = buildOrder[n].y;
			int i = a + squareCount;
			int j = b + squareCount;
			WorldCoor cellCoor = baseCoor;
			cellCoor.pos.x += a*cellSize;
			cellCoor.pos.y += b*cellSize;
			cellCoor.fix(world->getNodeSize());

			gRandom random(baseCoor.index.x * cellPerNode + baseX + a, baseCoor.index.y * cellPerNode + baseY + b);


			initNode(nodes[i][j], cellCoor, random);
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