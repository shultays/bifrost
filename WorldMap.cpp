
#include "gGame.h"
#include "PngExporter.h"
#include "WorldMap.h"
#include "gTexture.h"
#include "gGlobals.h"


WorldMap::WorldMap(float mapSize, int edgeCount) {
	this->mapSize = mapSize;
	this->edgeCount = edgeCount;
	this->nodeSize = mapSize / edgeCount;
	waterDrawable = NULL;
	terrainDrawable = NULL;

	heightMap.init(edgeCount);
	normalMap.init(edgeCount);
	colorMap.init(edgeCount);

	earthMap.setWorldMap(this);
	perlinMap.setWorldMap(this);
	detailMap.setWorldMap(this);

	texture = new gTexture("images/lena.png");
}



void WorldMap::build() {
	perlinMap.clear();
	earthMap.clear();

	float maxHeight = 10000.0f;

	perlinMap.addPerlinShell(10, 0.0f, 4 * maxHeight * 0.05f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(11, 0.0f, 4 * maxHeight * 0.05f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(13, 0.0f, 4 * maxHeight * 0.1f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(14, 0.0f, 4 * maxHeight * 0.2f, 0.85f, 2.0f);
	perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);
	perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);
	//perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);

	perlinMap.addPerlinShell(20, -1000.0, 0.0f, 0.5f);



	float remainingHeight = 0.4f * maxHeight;

	int start = 5;
	do {
		float h = remainingHeight*0.5f;
		perlinMap.addPerlinShell(start, 0.0f, h);
		remainingHeight -= h;
		start = (int)(start * 2.1f);
	} while (start < edgeCount);

	perlinMap.addPerlinShell(start, 0.0f, remainingHeight);

	int t = 1;
	while (mapSize / start > 0.05f) {
		float h = 10.0f / t;
		t++;
		detailMap.addPerlinShell(start, 0.0f, h);
		start = (int)(start * 2.1f);
	}



	earthMap.addPerlinShell(03, 0.0, 0.30f, 0.90f);
	earthMap.addPerlinShell(04, 0.0, 0.20f, 0.90f);
	earthMap.addPerlinShell(06, 0.0, 0.15f, 0.90f);
	earthMap.addPerlinShell(15, 0.0, 0.15f, 0.90f);
	earthMap.addPerlinShell(37, 0.0, 0.15f, 0.60f);
	earthMap.addPerlinShell(55, 0.0, 0.05f, 0.50f);
	earthMap.addPerlinShell(85, 0.0, 0.05f, 0.50f);

	SAFE_DELETE(terrainDrawable);
	SAFE_DELETE(waterDrawable);

	terrainDrawable = new gStaticIndexBufferedDrawable(VERTEX_PROP_COLOR | VERTEX_PROP_NORMAL | VERTEX_PROP_POSITION | VERTEX_PROP_UV, edgeCount*edgeCount, (edgeCount - 1)*(edgeCount - 1) * 6, false);
	waterDrawable = new gStaticTriangleVertexBufferDrawable(VERTEX_PROP_POSITION, 6, false);

	buildHeightMap();
	buildNormalMap();
	buildColorMap();
	buildBuffer();


	node.setWorldMap(this);
	node.build(Vec2(edgeCount*nodeSize / 2.0f, edgeCount*nodeSize / 2.0f), Vec2(edgeCount*nodeSize / 2.0f + nodeSize, edgeCount*nodeSize / 2.0f + nodeSize), 16);

	PngExporter::writeGridToPng("images/normalMap.png", normalMap, ExportTypeVec3AsNormal);
	PngExporter::writeGridToPng("images/heightMap.png", heightMap);
	PngExporter::writeGridToPng("images/earthMap.png", earthMap, edgeCount, Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0));
}

float WorldMap::getMapSize() {
	return mapSize;
}

float WorldMap::getNodeSize() {
	return nodeSize;
}

void WorldMap::buildHeightMap() {
	heightMap.setAll(0.0f);

	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			heightMap[i][j] = perlinMap.getHeightAt(WorldCoor(i, j, 0.5f, 0.5f));
		}
	}

	float min = FLT_MAX, max = FLT_MIN;

	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			min = gmin(min, heightMap[i][j]);
			max = gmax(max, heightMap[i][j]);
		}
	}

	// level land around water
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			heightMap[i][j] -= 1500.0f;
			if (heightMap[i][j] < 20.0f) {
				heightMap[i][j] = (heightMap[i][j] - 20.0f)*0.5f;
			} else if (heightMap[i][j] < 80.0f) {
				heightMap[i][j] = (heightMap[i][j] - 20.0f)* 80.0f / 60.0f;
			}
		}
	}

	//fix too small areas
	for (int i = 5; i < edgeCount - 5; i++) {
		for (int j = 5; j < edgeCount - 5; j++) {
			if (heightMap[i][j] < 0) {
				int waterToAccept = 20;
				for (int a = i - 5; a < i + 5; a++) {
					for (int b = j - 5; b < j + 5; b++) {
						if (heightMap[a][b] < 0) {
							waterToAccept--;
							if (!waterToAccept) break;
						}
					}
				}
				if (waterToAccept > 0) {
					heightMap[i][j] = -heightMap[i][j] + 0.1f;;
				}
			} else {
				int landToAccept = 20;
				for (int a = i - 5; a < i + 5; a++) {
					for (int b = j - 5; b < j + 5; b++) {
						if (heightMap[a][b] > 0) {
							landToAccept--;
							if (!landToAccept) break;
						}
					}
				}
				if (landToAccept > 0) {
					heightMap[i][j] = -heightMap[i][j] - 0.1f;;
				}
			}
		}
	}


	min = FLT_MAX, max = FLT_MIN;


	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			min = gmin(min, heightMap[i][j]);
			max = gmax(max, heightMap[i][j]);
		}
	}
}
void WorldMap::buildNormalMap() {
	for (int i = 1; i < edgeCount - 1; i++) {
		for (int j = 1; j < edgeCount - 1; j++) {
			float dt = 1.0f*nodeSize;
			float sx = perlinMap.getHeightAt(WorldCoor(i + 1, j)) - perlinMap.getHeightAt(WorldCoor(i - 1, j));
			float sy = perlinMap.getHeightAt(WorldCoor(i, j + 1)) - perlinMap.getHeightAt(WorldCoor(i, j - 1));


			normalMap[i][j] = Vec3(-sx, sy, 2.0f*dt);
			normalMap[i][j].normalize();
		}
	}
	for (int i = 0; i < edgeCount; i++) {
		normalMap[i][0] = Vec3(0.0f, 0.0f, 1.0f);
		normalMap[i][edgeCount - 1] = Vec3(0.0f, 0.0f, 1.0f);

		normalMap[edgeCount - 1][i] = Vec3(0.0f, 0.0f, 1.0f);
		normalMap[0][i] = Vec3(0.0f, 0.0f, 1.0f);
	}
}
void WorldMap::buildColorMap() {
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			float height = heightMap[i][j];
			float beachW = (2.0f - height) / 200.0f;
			saturate(beachW);
			float snowW = (height - 5000.0f) / 1000.0f;
			saturate(snowW);


			float forestW = earthMap.getHeightAt(WorldCoor(i, j));
			saturate(forestW);

			Vec3 finalColor = Vec3::zero();



			finalColor += lerpVec(Vec3(0.9f), Vec3(1.0f), forestW) * snowW;

			finalColor += lerpVec(Vec3::fromColor(0xB7A888), Vec3::fromColor(0x80765F), forestW) * beachW;

			float remainingWeight = 1.0f - snowW - beachW;


			finalColor += lerpVec(Vec3::fromColor(0x477519), Vec3::fromColor(0x663300), forestW) * remainingWeight;
			colorMap[i][j] = finalColor;
			/*
			if (height < 500) {
			colorMap[i][j] = 0.0f;
			} else {
			int t = (int)((height - 500) / 500.0f);
			colorMap[i][j] = Vec3(t / 5.0f);
			}*/

		}
	}
}

float WorldMap::getHeightAt(WorldCoor &coor) {
	int i, j;
	float dx, dy;
	float tx = coor.pos.x / nodeSize;
	float ty = coor.pos.y / nodeSize;
	i = (int)floor(tx) + coor.index.x;
	j = (int)floor(ty) + coor.index.y;
	dx = tx - i;
	dy = ty - j;

	// p2 -- p4
	// |  \  |
	// p0 -- p1

	float r;

	if (dx + dy < 1.0f) {
		float p0 = heightMap[i][j];
		float p1 = heightMap[i + 1][j];
		float p2 = heightMap[i][j + 1];


		r = p0 * (1.0f - dx - dy) + dx * p1 + dy * p2;
	} else {
		float p4 = heightMap[i + 1][j + 1];
		float p1 = heightMap[i + 1][j];
		float p2 = heightMap[i][j + 1];

		dx = 1.0f - dx;
		dy = 1.0f - dy;

		r = p4 * (1.0f - dx - dy) + dx * p2 + dy * p1;
	}
	//float t = detailMap.getHeightAt(pos);
	return r;
}

void WorldMap::buildBuffer() {


	*waterDrawable->getVertexPointerAt(0).position = Vec3(0.0f, 0.0f, 0.0f) * WORLD_MAP_SCALE;
	*waterDrawable->getVertexPointerAt(1).position = Vec3(+mapSize, 0.0f, 0.0f) * WORLD_MAP_SCALE;
	*waterDrawable->getVertexPointerAt(2).position = Vec3(0.0f, +mapSize, 0.0f) * WORLD_MAP_SCALE;
	*waterDrawable->getVertexPointerAt(3).position = Vec3(+mapSize, 0.0f, 0.0f) * WORLD_MAP_SCALE;
	*waterDrawable->getVertexPointerAt(4).position = Vec3(+mapSize, +mapSize, 0.0f) * WORLD_MAP_SCALE;
	*waterDrawable->getVertexPointerAt(5).position = Vec3(0.0f, +mapSize, 0.0f) * WORLD_MAP_SCALE;

	waterDrawable->build();

	waterDrawable->setConstantNormal(Vec3(0.0f, 0.0f, 1.0f));
	waterDrawable->setConstantColor(Vec4(0.0f, 0.0f, 0.5f, 0.8f));

	int k = 0;
	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			VertexPointer pointer = terrainDrawable->getVertexPointerAt(k++);
			*pointer.position = Vec3(i*nodeSize, j*nodeSize, heightMap[i][j] * 3.0f)*WORLD_MAP_SCALE;
			*pointer.normal = normalMap[i][j];
			*pointer.color = Vec4(colorMap[i][j], 1.0f);
			*pointer.uv = Vec2(((float)i) / edgeCount, ((float)j) / edgeCount);
		}
	}
	k = 0;
	for (int i = 0; i < (edgeCount - 1); i++) {
		for (int j = 0; j < (edgeCount - 1); j++) {
			terrainDrawable->setIndexAt(k++, i + (j + 1)*edgeCount);
			terrainDrawable->setIndexAt(k++, (i + 1) + j*edgeCount);
			terrainDrawable->setIndexAt(k++, i + j*edgeCount);


			terrainDrawable->setIndexAt(k++, i + (j + 1)*edgeCount);
			terrainDrawable->setIndexAt(k++, (i + 1) + (j + 1)*edgeCount);
			terrainDrawable->setIndexAt(k++, (i + 1) + j*edgeCount);
		}
	}

	terrainDrawable->build();
}


void WorldMap::render() {
	terrainDrawable->render();
	waterDrawable->render();
}
