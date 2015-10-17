#include "ForestGenerator.h"

#include "WorldMap.h"
#include "gRenderableGroup.h"
#include "TreeGenerator.h"
#include "gGlobals.h"
#include "gRandom.h"

class ForestNodeBuilder : public gSlaveWork {
public:
	ForestGenerator* forestGenerator;

	WorldCoor cellCoor;
	float cellSize;
	WorldMap* world;
	std::vector<gVertexBufferRenderable*> trees;

	gVertexBufferRenderable* renderable;

	gRandom random;
public:
	ForestNodeBuilder(ForestGenerator* forestGenerator, WorldMap*world, gVertexBufferRenderable* renderable, WorldCoor cellCoor, float cellSize, gRandom random, int priority) {
		this->forestGenerator = forestGenerator;
		this->cellCoor = cellCoor;
		this->cellSize = cellSize;
		this->renderable = renderable;
		this->world = world;
		this->random = random;
		this->priority = priority;
	}
	virtual void runOnSlave() override {
		HeightCacher cacher;
		cacher.init(1);

		std::vector<TreeTriangle> vertices;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				WorldCoor treeCoor = cellCoor;
				treeCoor.pos += Vec2(i*cellSize / 4, j*cellSize / 4);
				treeCoor.pos.x += random.randFloat(-cellSize / 8, cellSize / 8);
				treeCoor.pos.y += random.randFloat(-cellSize / 8, cellSize / 8);
				float treeProb = world->getTreeProbabilityAt(treeCoor, cacher);

				if (treeProb > 0.0f || true) {
					Vec3 treePos = world->toGamePos(treeCoor, cacher);

					TreeGenerator::generateTree2(treePos, vertices, random);
				}
			}
		}

		renderable->init(VERTEX_PROP_COLOR | VERTEX_PROP_POSITION | VERTEX_PROP_NORMAL, vertices.size() * 3, true);

		for (unsigned i = 0; i < vertices.size(); i++) {
			VertexPointer pointer0 = renderable->getVertexPointerAt(i * 3 + 0);
			VertexPointer pointer1 = renderable->getVertexPointerAt(i * 3 + 1);
			VertexPointer pointer2 = renderable->getVertexPointerAt(i * 3 + 2);

			*pointer0.position = vertices[i].vertices[0];
			*pointer1.position = vertices[i].vertices[1];
			*pointer2.position = vertices[i].vertices[2];

			*pointer0.color = *pointer1.color = *pointer2.color = Vec4(vertices[i].color, 1.0f);

			Vec3 normal = Vec3::cross(vertices[i].vertices[1] - vertices[i].vertices[0], vertices[i].vertices[2] - vertices[i].vertices[0]);
			normal.normalize();
			*pointer0.normal = *pointer1.normal = *pointer2.normal = normal;
		}
	}
	virtual void runOnMain() override {
		renderable->build();

		forestGenerator->waitingJobs--;

		delete this;
	}
};

ForestGenerator::ForestGenerator(WorldMap* world) {

	DetailCreator::init(world, world->getNodeSize() / 20, 3);
}

void ForestGenerator::initNode(gVertexBufferRenderable*& node, WorldCoor& nodeCoor, gRandom& random, int a, int b) {
	node = new gVertexBufferRenderable();

	gears.addSlaveWork(new ForestNodeBuilder(this, world, node, nodeCoor, cellSize, random, 1000 - (abs(a) + abs(b) * 4)));
	waitingJobs++;
}

void ForestGenerator::deleteNode(gVertexBufferRenderable*& node) {
	SAFE_DELETE(node);
}
