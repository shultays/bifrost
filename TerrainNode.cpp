
#include "TerrainNode.h"
#include "gStaticBufferedDrawables.h"
#include "WorldMap.h"
#include "gGlobals.h"


void TerrainNode::build(IntVec2 index, int edgeCount) {
	SAFE_DELETE(drawable);

	drawable = new gStaticIndexBufferedDrawable(VERTEX_PROP_POSITION, edgeCount*edgeCount, (edgeCount - 1)*(edgeCount - 1) * 6, false);


	heightMap.init(edgeCount);
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			Vec2 pos(world->getNodeSize() * (float)i / (edgeCount - 1), world->getNodeSize() * (float)j / (edgeCount - 1));
			WorldCoor coor(index, pos);
			float h = world->getHeightAt(coor);
			heightMap[i][j] = h;
		}
	}


	int k = 0;
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			Vec2 pos(world->getNodeSize() * (float)i / (edgeCount - 1), world->getNodeSize() * (float)j / (edgeCount - 1));
			VertexPointer pointer = drawable->getVertexPointerAt(k++);
			*pointer.position = Vec3(pos, heightMap[i][j]);
		}
	}
	k = 0;
	for (int i = 0; i < (edgeCount - 1); i++) {
		for (int j = 0; j < (edgeCount - 1); j++) {
			drawable->setIndexAt(k++, i + (j + 1)*edgeCount);
			drawable->setIndexAt(k++, (i + 1) + j*edgeCount);
			drawable->setIndexAt(k++, i + j*edgeCount);

			drawable->setIndexAt(k++, i + (j + 1)*edgeCount);
			drawable->setIndexAt(k++, (i + 1) + (j + 1)*edgeCount);
			drawable->setIndexAt(k++, (i + 1) + j*edgeCount);
		}
	}

	drawable->setConstantNormal(Vec3(0.0f, 0.0f, 1.0f));
	drawable->setConstantColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));

	frame.makeIdentity();
	frame.translateBy(Vec3(0.0f, 0.0f, 0.1f));

	//frame.translateBy(Vec3(index.x * world->getNodeSize(), index.y * world->getNodeSize(), 0.0f));

	drawable->build();
}

void TerrainNode::render() {

	drawable->render();
}
