#include "ForestGenerator.h"



ForestGenerator::ForestGenerator(WorldMap* world) {
	this->world = world;
	cellPerNode = 2;
	squareCount = 5;

	cellSize = world->getNodeSize() / cellPerNode;
	nodes.init(squareCount + squareCount + 1, squareCount + squareCount + 1, nullptr);
}

void ForestGenerator::updateMap(WorldCoor& coor) {

}

void ForestGenerator::initMap(WorldCoor& coor) {
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
