#include "TerrainNode.h"
#include "gIndexBufferedDrawable.h"
#include "WorldMap.h"
#include "gGlobals.h"


TerrainNode::TerrainNode(WorldMap *world) : gRenderable(true, 2) {
	drawable = NULL;
	this->world = world;
	shader = world->shader;
	isBuilt = false;
	cropArea.setZero();
}

void TerrainNode::build(WorldCoor start, Vec2 size, int edgeCount) {
	SAFE_DELETE(drawable);

	drawable = new gVertexBufferRenderable(VERTEX_PROP_COLOR | VERTEX_PROP_NORMAL | VERTEX_PROP_POSITION, (edgeCount - 1)*(edgeCount - 1) * 6, false);

	HeightCacher cacher;
	cacher.init(16);

	struct VertexData {
		Vec3 pos;
		Vec3 color;
	};


	Grid<VertexData> grid;

	grid.init(edgeCount);

	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {

			Vec2 pos(size.x * (float)i / (edgeCount - 1), size.y * (float)j / (edgeCount - 1));
			WorldCoor coor = start;
			coor.pos += pos;
			coor.fix(world->getNodeSize());

			float h = world->getHeightAt(coor, cacher);

			IntVec2 shift = coor.index - world->anchorPos;

			grid[i][j].pos = Vec3(coor.pos.x + shift.x * world->getNodeSize(), coor.pos.y + shift.y * world->getNodeSize(), h);

			float dx = size.x / (edgeCount - 1);
			float dy = size.y / (edgeCount - 1);


			float tx = grid[i][j].pos.x / world->getNodeSize();
			float ty = grid[i][j].pos.y / world->getNodeSize();

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


			grid[i][j].color = r;
		}

	}

	int k = 0;
	for (int i = 0; i < edgeCount - 1; i++) {
		for (int j = 0; j < edgeCount - 1; j++) {

			VertexPointer pointer0 = drawable->getVertexPointerAt(k++);
			VertexPointer pointer1 = drawable->getVertexPointerAt(k++);
			VertexPointer pointer2 = drawable->getVertexPointerAt(k++);


			*pointer0.position = grid[i][j].pos;
			*pointer1.position = grid[i + 1][j].pos;
			*pointer2.position = grid[i + 1][j + 1].pos;

			*pointer0.normal = *pointer1.normal = *pointer2.normal = Vec3::cross(*pointer1.position - *pointer0.position, *pointer2.position - *pointer0.position);

			Vec3 color = (grid[i][j].color + grid[i + 1][j].color + grid[i + 1][j + 1].color) / 3.0f;

			*pointer0.color = Vec4(color, 1.0f);
			*pointer1.color = Vec4(color, 1.0f);
			*pointer2.color = Vec4(color, 1.0f);

			pointer0 = drawable->getVertexPointerAt(k++);
			pointer1 = drawable->getVertexPointerAt(k++);
			pointer2 = drawable->getVertexPointerAt(k++);


			*pointer0.position = grid[i + 1][j + 1].pos;
			*pointer1.position = grid[i][j + 1].pos;
			*pointer2.position = grid[i][j].pos;

			*pointer0.normal = *pointer1.normal = *pointer2.normal = Vec3::cross(*pointer1.position - *pointer0.position, *pointer2.position - *pointer0.position);

			color = (grid[i][j].color + grid[i][j + 1].color + grid[i + 1][j + 1].color) / 3.0f;

			*pointer0.color = Vec4(color, 1.0f);
			*pointer1.color = Vec4(color, 1.0f);
			*pointer2.color = Vec4(color, 1.0f);
		}
	}

	startPoint = grid[0][0].pos.vec2;
	endPoint = grid[edgeCount - 1][edgeCount - 1].pos.vec2;
	isBuilt = false;
}

void TerrainNode::buildMesh() {
	isBuilt = true;
	drawable->build();
}

void TerrainNode::render() {
	if (isBuilt) {
		shader->setUniform("uDiscardArea", cropArea);
		drawable->render();
	}
}
