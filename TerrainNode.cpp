
#include "TerrainNode.h"
#include "gStaticBufferedDrawables.h"
#include "WorldMap.h"
#include "gGlobals.h"


void TerrainNode::build(Vec2 min, Vec2 max, int edgeCount) {
	SAFE_DELETE(drawable);

	drawable = new gStaticIndexBufferedDrawable(VERTEX_PROP_POSITION, edgeCount*edgeCount, (edgeCount - 1)*(edgeCount - 1) * 6, false);


	heightMap.init(edgeCount);
	Vec2 diff = max - min;
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			Vec2 p = min;
			p.x += diff.x * (float)i / (edgeCount - 1);
			p.y += diff.y * (float)j / (edgeCount - 1);
			float h = world->getHeightAt(p);
			heightMap[i][j] = h;
		}
	}


	int k = 0;
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			Vec2 p = min;

			p.x += diff.x * (float)i / (edgeCount - 1);
			p.y += diff.y * (float)j / (edgeCount - 1);

			VertexPointer pointer = drawable->getVertexPointerAt(k++);
			*pointer.position = Vec3(p, heightMap[i][j]);

			printf("%f %f %f\n", pointer.position->x, pointer.position->y, pointer.position->z);
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

	drawable->build();
}

void TerrainNode::render() {

	drawable->render();
}
