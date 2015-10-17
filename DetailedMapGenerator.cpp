#include "DetailedMapGenerator.h"
#include "TerrainNode.h"
#include "WorldMap.h"
#include "gSlave.h"
#include "gGlobals.h"

class DetailedMapNodeBuilder : public gSlaveWork {
public:
	DetailedMapGenerator* mapController;
	TerrainNode* node;
	WorldCoor cellCoor;
	float cellSize;
	int edgePerCell;

	DetailedMapNodeBuilder(DetailedMapGenerator* mapController, TerrainNode* node, WorldCoor cellCoor, float cellSize, int edgePerCell, int priority) {
		this->mapController = mapController;
		this->node = node;
		this->cellCoor = cellCoor;
		this->cellSize = cellSize;
		this->edgePerCell = edgePerCell;
		this->priority = priority;
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

DetailedMapGenerator::DetailedMapGenerator(WorldMap* world, int squareCount, int cellPerNode, int edgePerCell) {
	this->world = world;
	this->edgePerCell = edgePerCell;
	cellDetail = cellSize / edgePerCell;
	waitingJobs = 0;

	DetailCreator::init(world, cellPerNode, squareCount);
}

void DetailedMapGenerator::tick(float dt) {
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


Vec4 DetailedMapGenerator::getTerrainArea() const {
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

void DetailedMapGenerator::initNode(TerrainNode*& node, WorldCoor& nodeCoor, gRandom& random, int a, int b) {
	node = new TerrainNode(world);
	gears.addSlaveWork(new DetailedMapNodeBuilder(this, node, nodeCoor, cellSize, edgePerCell, 1001 - (abs(a) + abs(b)) * 4));
	waitingJobs++;
}

void DetailedMapGenerator::deleteNode(TerrainNode*& node) {
	SAFE_DELETE(node);
}
