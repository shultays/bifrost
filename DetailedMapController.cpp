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

DetailedMapController::DetailedMapController(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell) : DetailCreator<TerrainNode>(world, cellPerNode, squareCount) {
	this->world = world;
	this->edgePerCell = edgePerCell;
	cellDetail = cellSize / edgePerCell;
	waitingJobs = 0;
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

void DetailedMapController::initNode(TerrainNode*& node, WorldCoor& nodeCoor, gRandom& random) {
	node = new TerrainNode(world);
	gears.addSlaveWork(new NodeBuilder(this, node, nodeCoor, cellSize, edgePerCell));
	waitingJobs++;
}

void DetailedMapController::deleteNode(TerrainNode*& node) {
	SAFE_DELETE(node);
}
