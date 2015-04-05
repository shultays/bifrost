#include "DetailedMapController.h"
#include "TerrainNode.h"
#include "WorldMap.h"

DetailedMapController::DetailedMapController(WorldMap* world) {
	this->world = world;


	squareCount = 16;
	cellPerNode = 1024;
	cellSize = world->getNodeSize() / cellPerNode;
	edgePerCell = 2;
	cellDetail = cellSize / edgePerCell;

	nodes.init(squareCount*squareCount + 1, squareCount*squareCount + 1, nullptr);
}

IntVec2 DetailedMapController::coorToIndex(WorldCoor& coor) {
	IntVec2 r = coor.index * cellPerNode;
	r.x += (int)(coor.pos.x / cellSize);
	r.y += (int)(coor.pos.y / cellSize);
	return r;
}

void DetailedMapController::updateMap(WorldCoor& coor) {


	WorldCoor baseCoor = coor;
	baseCoor.pos.x = ((int)(baseCoor.pos.x / cellSize)) * cellSize;
	baseCoor.pos.y = ((int)(baseCoor.pos.y / cellSize)) * cellSize;


	WorldCoor diff = baseCoor - oldCoor;
	Vec2 diffPos = diff.pos + diff.index.toVec() * world->getNodeSize();

	if (diffPos.x > cellSize) {
		for (int a = -squareCount; a <= squareCount; a++) {
			for (int b = -squareCount; b <= squareCount; b++) {
				int i = a + squareCount;
				int j = b + squareCount;

				if (a < -squareCount + 2) {
					SAFE_DELETE(nodes[i][j]);
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
					nodes[i][j] = new TerrainNode(world);
					nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
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
					SAFE_DELETE(nodes[i][j]);
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
					nodes[i][j] = new TerrainNode(world);
					nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
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
					SAFE_DELETE(nodes[i][j]);
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
					nodes[i][j] = new TerrainNode(world);
					nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
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
					SAFE_DELETE(nodes[i][j]);
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
					nodes[i][j] = new TerrainNode(world);
					nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
				}
			}
		}

		oldCoor.index.y = baseCoor.index.y;
		oldCoor.pos.y = baseCoor.pos.y;
	}
}

void DetailedMapController::initMap(WorldCoor& coor) {
	for (int i = 0; i < squareCount*squareCount + 1; i++) {
		for (int j = 0; j < squareCount*squareCount + 1; j++) {
			SAFE_DELETE(nodes[i][j]);
		}
	}


	IntVec3 centerIndex;

	WorldCoor baseCoor = coor;
	baseCoor.pos.x = ((int)(baseCoor.pos.x / cellSize)) * cellSize;
	baseCoor.pos.y = ((int)(baseCoor.pos.y / cellSize)) * cellSize;

	oldCoor = baseCoor;
	oldIndex = coorToIndex(oldCoor);

	for (int a = -squareCount; a <= squareCount; a++) {
		for (int b = -squareCount; b <= squareCount; b++) {
			int i = a + squareCount;
			int j = b + squareCount;
			WorldCoor cellCoor = baseCoor;
			cellCoor.pos.x += a*cellSize;
			cellCoor.pos.y += b*cellSize;
			cellCoor.fix(world->getNodeSize());
			nodes[i][j] = new TerrainNode(world);
			nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
		}
	}

}