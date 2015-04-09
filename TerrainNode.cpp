
#include "TerrainNode.h"
#include "gStaticBufferedDrawables.h"
#include "WorldMap.h"
#include "gGlobals.h"


void TerrainNode::build(WorldCoor start, Vec2 size, int edgeCount) {
	SAFE_DELETE(drawable);

	drawable = new gStaticIndexBufferedDrawable(VERTEX_PROP_COLOR | VERTEX_PROP_NORMAL | VERTEX_PROP_POSITION | VERTEX_PROP_UV, edgeCount*edgeCount, (edgeCount - 1)*(edgeCount - 1) * 6, false);

	HeightCacher cacher;
	cacher.init(16);

	int k = 0;
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			VertexPointer pointer = drawable->getVertexPointerAt(k++);


			Vec2 pos(size.x * (float)i / (edgeCount - 1), size.y * (float)j / (edgeCount - 1));
			WorldCoor coor = start;
			coor.pos += pos;
			coor.fix(world->getNodeSize());

			float h = world->getHeightAt(coor, cacher);

			IntVec2 shift = coor.index - world->anchorPos;

			*pointer.position = Vec3(coor.pos.x + shift.x * world->getNodeSize(), coor.pos.y + shift.y * world->getNodeSize(), h);


			float dx = size.x / (edgeCount - 1);
			float dy = size.y / (edgeCount - 1);

			float sx = world->getHeightAt(WorldCoor(coor, 0, 0, dx, 0), cacher) - world->getHeightAt(WorldCoor(coor, 0, 0, -dx, 0), cacher);
			float sy = world->getHeightAt(WorldCoor(coor, 0, 0, 0, dy), cacher) - world->getHeightAt(WorldCoor(coor, 0, 0, 0, -dy), cacher);

			*pointer.normal = Vec3(-sx, sy, 2.0f*dx);
			pointer.normal->normalize();



			float tx = pointer.position->x / world->getNodeSize();
			float ty = pointer.position->y / world->getNodeSize();

			Vec3 r;
			if (dx + dy < 1.0f) {
				Vec3 p0 = world->colorMap[coor.index.x][coor.index.y];
				Vec3 p1 = world->colorMap[coor.index.x + 1][coor.index.y];
				Vec3 p2 = world->colorMap[coor.index.x][coor.index.y + 1];


				r = p0 * (1.0f - tx - ty) + p1 * tx + p2 * ty;
			} else {
				Vec3 p4 = world->colorMap[coor.index.x + 1][coor.index.y + 1];
				Vec3 p1 = world->colorMap[coor.index.x + 1][coor.index.y];
				Vec3 p2 = world->colorMap[coor.index.x][coor.index.y + 1];

				dx = 1.0f - dx;
				dy = 1.0f - dy;

				r = p4 * (1.0f - tx - ty) + p2 * tx + p1 * ty;
			}


			*pointer.color = Vec4(r, 1.0f);
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


	frame.makeIdentity();
	frame.translateBy(Vec3(0.0f, 0.0f, 0.1f));

	//frame.translateBy(Vec3(index.x * world->getNodeSize(), index.y * world->getNodeSize(), 0.0f));

	drawable->build();
}

void TerrainNode::render() {
	drawable->render();
}
