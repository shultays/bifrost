#include "DetailedMapController.h"
#include "TerrainNode.h"
#include "WorldMap.h"
#include "gSlave.h"
#include "gGlobals.h"

class NodeBuilder : public gSlaveWork {
public:
	DetailedMapController* mapController;
	TerrainNode* node;
	WorldCoor cellCoor;
	float cellSize;
	int edgePerCell;

	NodeBuilder(DetailedMapController* mapController, TerrainNode* node, WorldCoor cellCoor, float cellSize, int edgePerCell) {
		this->mapController = mapController;
		this->node = node;
		this->cellCoor = cellCoor;
		this->cellSize = cellSize;
		this->edgePerCell = edgePerCell;
	}
	virtual void runOnSlave() override {

		node->build(cellCoor, Vec2(cellSize), edgePerCell);
	}
	virtual void runOnMain() override {
		node->buildMesh();
		mapController->waitingJobs--;
		delete this;
	}
};

DetailedMapController::DetailedMapController(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell) {
	this->world = world;

	this->squareCount = squareCount;
	this->cellPerNode = cellPerNode;
	this->edgePerCell = edgePerCell;

	waitingJobs = 0;
	cellSize = world->getNodeSize() / cellPerNode;
	cellDetail = cellSize / edgePerCell;

	nodes.init(squareCount + squareCount + 1, squareCount + squareCount + 1, nullptr);
}

void DetailedMapController::tick(float dt) {
	if (cropArea.isNonZero()) {
		for (int a = -squareCount; a <= squareCount; a++) {
			for (int b = -squareCount; b <= squareCount; b++) {
				int i = a + squareCount;
				int j = b + squareCount;
				if (nodes[i][j]) {
					nodes[i][j]->setDiscardArea(cropArea);
				}
			}
		}
	}
}

IntVec2 DetailedMapController::coorToIndex(WorldCoor& coor) {
	IntVec2 r = coor.index * cellPerNode;
	r.x += (int)(coor.pos.x / cellSize);
	r.y += (int)(coor.pos.y / cellSize);
	return r;
}

void DetailedMapController::updateMap(WorldCoor& coor) {
	if (waitingJobs > 0) {
		return;
	}
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
					gears.addSlaveWork(new NodeBuilder(this, nodes[i][j], cellCoor, cellSize, edgePerCell));
					waitingJobs++;
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
					gears.addSlaveWork(new NodeBuilder(this, nodes[i][j], cellCoor, cellSize, edgePerCell));
					waitingJobs++;
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
					gears.addSlaveWork(new NodeBuilder(this, nodes[i][j], cellCoor, cellSize, edgePerCell));
					waitingJobs++;
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
					gears.addSlaveWork(new NodeBuilder(this, nodes[i][j], cellCoor, cellSize, edgePerCell));
					waitingJobs++;
				}
			}
		}

		oldCoor.index.y = baseCoor.index.y;
		oldCoor.pos.y = baseCoor.pos.y;
	}
}

void DetailedMapController::initMap(WorldCoor& coor) {
	for (int i = 0; i < squareCount + squareCount + 1; i++) {
		for (int j = 0; j < squareCount + squareCount + 1; j++) {
			SAFE_DELETE(nodes[i][j]);
		}
	}


	IntVec3 centerIndex;

	WorldCoor baseCoor = coor;
	baseCoor.pos.x = ((int)(baseCoor.pos.x / cellSize)) * cellSize;
	baseCoor.pos.y = ((int)(baseCoor.pos.y / cellSize)) * cellSize;

	oldCoor = baseCoor;
	oldIndex = coorToIndex(oldCoor);
	waitingJobs = 0;
	for (int a = -squareCount; a <= squareCount; a++) {
		for (int b = -squareCount; b <= squareCount; b++) {
			int i = a + squareCount;
			int j = b + squareCount;
			WorldCoor cellCoor = baseCoor;
			cellCoor.pos.x += a*cellSize;
			cellCoor.pos.y += b*cellSize;
			cellCoor.fix(world->getNodeSize());
			nodes[i][j] = new TerrainNode(world);
			//nodes[i][j]->build(cellCoor, Vec2(cellSize), edgePerCell);
			//nodes[i][j]->buildMesh();

			gears.addSlaveWork(new NodeBuilder(this, nodes[i][j], cellCoor, cellSize, edgePerCell));
			waitingJobs++;
		}
	}

}

Vec4 DetailedMapController::getTerrainArea() const {
	int end = squareCount + squareCount;
	if (nodes[0][0] && nodes[0][0]->getIsBuilt() && nodes[end][end] && nodes[end][end]->getIsBuilt()) {
		Vec4 r;
		r[0] = nodes[0][0]->getMin().x;
		r[1] = nodes[0][0]->getMin().y;
		r[2] = nodes[end][end]->getMax().x;
		r[3] = nodes[end][end]->getMax().y;

		return r;
	}

	return Vec4::zero();
}
