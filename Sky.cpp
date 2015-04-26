#include "Sky.h"
#include "WorldMap.h"
#include "gGlobals.h"
#include "TreeGenerator.h"

Sky::Sky(WorldMap* world) {
	this->skyRandom.setSeed(random.randInt());
	this->world = world;
	this->renderable = nullptr;
}

Sky::~Sky() {
	deleteSky();
}

void Sky::updateSky(WorldCoor& coor) {

}

void Sky::initSky(WorldCoor& coor) {
	Vec3 center = world->toGamePos(coor);
	std::vector<TreeTriangle> triangles;

	for (int i = 0; i < 20; i++) {
		Vec3 pos = center + Vec3(skyRandom.randFloat(-1500, 1500), skyRandom.randFloat(-1500, 1500), 1000.0f);

		TreeGenerator::generateCloud(pos, Vec3(1.0f, 0.0f, 0.0f), triangles, skyRandom);
	}

	renderable = new gVertexBufferRenderable(VERTEX_PROP_COLOR | VERTEX_PROP_POSITION | VERTEX_PROP_NORMAL, triangles.size() * 3, true);

	for (unsigned i = 0; i < triangles.size(); i++) {
		VertexPointer pointer0 = renderable->getVertexPointerAt(i * 3 + 0);
		VertexPointer pointer1 = renderable->getVertexPointerAt(i * 3 + 1);
		VertexPointer pointer2 = renderable->getVertexPointerAt(i * 3 + 2);

		*pointer0.position = triangles[i].vertices[0];
		*pointer1.position = triangles[i].vertices[1];
		*pointer2.position = triangles[i].vertices[2];

		*pointer0.color = *pointer1.color = *pointer2.color = Vec4(triangles[i].color, 1.0f);

		Vec3 normal = Vec3::cross(triangles[i].vertices[1] - triangles[i].vertices[0], triangles[i].vertices[2] - triangles[i].vertices[0]);
		normal.normalize();
		*pointer0.normal = *pointer1.normal = *pointer2.normal = -normal * 0.7f;
	}

	renderable->build();
}

void Sky::deleteSky() {
	SAFE_DELETE(renderable);
}
