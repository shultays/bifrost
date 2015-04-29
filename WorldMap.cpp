
#include "gGame.h"
#include "PngExporter.h"
#include "WorldMap.h"
#include "gTexture.h"
#include "gGlobals.h"
#include "gCamera.h"
#include "gCamera.h"
#include <queue>
#include <set>
#define DRAINAGE_NODE_SIZE 1500.0f

WorldMap::WorldMap(float mapSize, int edgeCount) : gRenderable(true, 1) {
	this->mapSize = mapSize;
	this->edgeCount = edgeCount;
	this->nodeSize = mapSize / edgeCount;
	waterDrawable = NULL;
	terrainDrawable = NULL;

	drainageEdgeCount = (int)(mapSize / DRAINAGE_NODE_SIZE);


	heightMap.init(edgeCount);
	normalMap.init(edgeCount);
	colorMap.init(edgeCount);
	drainageGrid.init(drainageEdgeCount);
	islandMap.init(drainageEdgeCount);

	earthMap.setWorldMap(this);
	perlinMap.setWorldMap(this);
	detailMap.setWorldMap(this);
	isScaled = false;
	//texture = new gTexture("images/lena.png");

}



void WorldMap::build() {
	debugRenderer.clear();
	perlinMap.clear();
	earthMap.clear();

	float maxHeight = 30000.0f;

	perlinMap.addPerlinShell(10, 0.0f, maxHeight * 0.05f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(11, 0.0f, maxHeight * 0.05f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(13, 0.0f, maxHeight * 0.1f, 0.95f, 2.0f);
	perlinMap.addPerlinShell(14, 0.0f, maxHeight * 0.2f, 0.85f, 2.0f);
	perlinMap.addPerlinShell(15, 0.0f, maxHeight * 0.1f, 0.75f, 2.0f);
	perlinMap.addPerlinShell(15, 0.0f, maxHeight * 0.1f, 0.75f, 2.0f);
	//perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);

	perlinMap.addPerlinShell(20, -1000.0, 0.0f, 0.5f);



	float remainingHeight = 0.2f * maxHeight;

	int start = 5;
	do {
		float h = remainingHeight*0.5f;
		perlinMap.addPerlinShell(start, 0.0f, h);
		remainingHeight -= h;
		start = (int)(start * 2.1f);
	} while (start < edgeCount);

	perlinMap.addPerlinShell(start, 0.0f, remainingHeight);

	int t = 1;
	float totalH = 0.0f;
	while (mapSize / start > 0.1f) {
		float d = mapSize / start;
		float h = 100.0f * d / 1000.0f;
		t++;
		detailMap.addPerlinShell(start, 0.0f, h);
		start = (int)(start * 2.1f);
		totalH += h;
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

	terrainDrawable = new gVertexBufferRenderable(VERTEX_PROP_COLOR | VERTEX_PROP_NORMAL | VERTEX_PROP_POSITION, (edgeCount - 1)*(edgeCount - 1) * 6, false);
	waterDrawable = new gVertexBufferRenderable(VERTEX_PROP_POSITION, 6, false);


	mainCacher.init(16);

	printf("buildHeightMap.\n");
	buildHeightMap();
	printf("buildNormalMap.\n");
	buildNormalMap();
	printf("buildDrainage.\n");
	//buildDrainage();
	printf("buildRivers.\n");
	//buildRivers();
	printf("buildColorMap.\n");
	buildColorMap();
	printf("buildBuffer.\n");
	buildBuffer();

	PngExporter::writeGridToPng("images/normalMap.png", normalMap, ExportTypeVec3AsNormal);
	PngExporter::writeGridToPng("images/heightMap.png", heightMap);
	PngExporter::writeGridToPng("images/earthMap.png", earthMap, edgeCount, Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0));


	Grid<float> drainageVals;
	drainageVals.init(drainageEdgeCount);

	for (int i = 0; i < drainageEdgeCount; i++) {
		for (int j = 0; j < drainageEdgeCount; j++) {
			drainageVals[i][j] = sqrt(drainageGrid[i][j].drainage);
		}
	}
	PngExporter::writeGridToPng("images/drainage.png", drainageVals);
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

	const int areaToCheck = 2;
	int minArea = 5;
	//fix too small areas
	for (int i = areaToCheck; i <= edgeCount - areaToCheck; i++) {
		for (int j = areaToCheck; j <= edgeCount - areaToCheck; j++) {
			if (heightMap[i][j] < 0.0f) {
				int waterToAccept = minArea;
				for (int a = i - areaToCheck; a < i + areaToCheck; a++) {
					for (int b = j - areaToCheck; b < j + areaToCheck; b++) {
						if (heightMap[a][b] < -100.0f) {
							waterToAccept--;
							if (!waterToAccept) break;
						}
					}
				}
				if (waterToAccept > 0) {
					heightMap[i][j] = -heightMap[i][j] + 0.1f;;
				}
			} else {
				int landToAccept = minArea;
				for (int a = i - areaToCheck; a < i + areaToCheck; a++) {
					for (int b = j - areaToCheck; b < j + areaToCheck; b++) {
						if (heightMap[a][b] > 100.0f) {
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

	islandMap.setZero();

	int nextLandID = 1;
	int nextWaterID = -1;


	auto paint = [&] (const IntVec2& startPos, int toSet, int setIf, std::unordered_map<int, IntVec2>& edges, std::vector<IntVec2>& added) {
		std::queue<IntVec2> queue;
		queue.push(startPos);

		edges.clear();
		added.clear();
		islandMap[startPos] = toSet;

		while(queue.size()){
			IntVec2 pos = queue.front();
			queue.pop();
			added.push_back(pos);
			for(int i=0; i<IntVec2::sideCount(); i++){
				IntVec2 side = pos.getSide(i);
				if(heightMap.isValid(side)){
					if(islandMap[side] == setIf){
						if(same_sign(toSet, heightMap[side])){
							islandMap[side] = toSet;
							queue.push(side);
						}
					}else if(islandMap[side] != toSet){
						edges[islandMap[side]] = side;
					}
				}
			}
		}
	};

	std::unordered_map<int, int> sizes;

	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			if(islandMap[i][j] == 0){
				IntVec2 startPos = IntVec2(i, j);
				int id;
				if(heightMap[i][j] > 0.0f){
					id = nextLandID++;
				}else{
					id = nextWaterID--;
				}
				sizes[id] = 0;

				std::unordered_map<int, IntVec2> edges;
				std::vector<IntVec2> added;

				paint(startPos, id, 0, edges, added);

				if(added.size() < 6){
					for(unsigned k=0; k<added.size(); k++){
						heightMap[added[k]] = -heightMap[added[k]] - 0.1f;
					}
				
					int oldId = id;
					id = edges.begin()->first;

					for (auto it=edges.begin(); it!=edges.end(); ++it){
						if(abs(it->first) < id){
							id = it->first;
						}
					}
					std::unordered_map<int, IntVec2> edgesTemp;
					std::vector<IntVec2> addedTemp;

					for (auto it=edges.begin(); it!=edges.end(); ++it){
						if(id != it->first){
							if(same_sign(heightMap[startPos], heightMap[it->second])){
								paint(it->second, 0, it->first, edgesTemp, addedTemp);
							}
						}
					}

					paint(startPos, 0, oldId, edgesTemp, addedTemp);
					paint(startPos, id, 0, edgesTemp, addedTemp);

				}else{
					debugRenderer.addLine(toWorldGamePos(i, j), toWorldGamePos(i, j) + Vec3(0.0f, 0.0f, 1.0f), id>0?0xFFFFFFFF : 0xFFFF0000, 100.0f);
				}
			}
		}
	}

	for (int i = 0; i < edgeCount; i++) {
		for (int j = 0; j < edgeCount; j++) {
			sizes[islandMap[i][j]]++;
		}
	}
	
	int totalSize = 0;
	
	for (auto it=sizes.begin(); it!=sizes.end(); ++it){
		totalSize += it->second;
		printf("%s %d count %d\n", it->second>0?"land": "watr", it->first, it->second);
	}
	printf("total %d (%d)\n", totalSize, edgeCount * edgeCount);
	
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

float WorldMap::getHeightAt(WorldCoor &coor, HeightCacher& cacher) const {
	coor.fix(nodeSize);
	if (coor.h != INVALID_HEIGHT) {
		//return coor.h;
	}
	float r;

	if (cacher.getCachedHeight(coor, r)) {
		return r;
	}

	int i, j;
	i = coor.index.x;
	j = coor.index.y;
	float dx = coor.pos.x / nodeSize;
	float dy = coor.pos.y / nodeSize;

	// p2 -- p4
	// |  \  |
	// p0 -- p1


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
	float t = detailMap.getHeightAt(coor);


	cacher.cacheHeight(coor, r + t);

	return coor.h = r + t;
}

void WorldMap::buildBuffer() {
	*waterDrawable->getVertexPointerAt(0).position = Vec3(0.0f, 0.0f, 0.0f);
	*waterDrawable->getVertexPointerAt(1).position = Vec3((float)edgeCount, 0.0f, 0.0f);
	*waterDrawable->getVertexPointerAt(2).position = Vec3(0.0f, (float)edgeCount, 0.0f);
	*waterDrawable->getVertexPointerAt(3).position = Vec3((float)edgeCount, 0.0f, 0.0f);
	*waterDrawable->getVertexPointerAt(4).position = Vec3((float)edgeCount, (float)edgeCount, 0.0f);
	*waterDrawable->getVertexPointerAt(5).position = Vec3(0.0f, (float)edgeCount, 0.0f);

	waterDrawable->build();

	waterDrawable->setConstantNormal(Vec3(0.0f, 0.0f, 1.0f));
	waterDrawable->setConstantColor(Vec4(0.0f, 0.0f, 0.5f, 0.8f));

	int k = 0;
	for (int i = 0; i < edgeCount - 1; i++) {
		for (int j = 0; j < edgeCount - 1; j++) {

			VertexPointer pointer0 = terrainDrawable->getVertexPointerAt(k++);
			VertexPointer pointer1 = terrainDrawable->getVertexPointerAt(k++);
			VertexPointer pointer2 = terrainDrawable->getVertexPointerAt(k++);


			*pointer0.position = toWorldGamePos(i, j);
			*pointer1.position = toWorldGamePos(i+1, j);
			*pointer2.position = toWorldGamePos(i+1, j+1);

			*pointer0.normal = *pointer1.normal = *pointer2.normal = Vec3::cross(*pointer1.position - *pointer0.position, *pointer2.position - *pointer0.position);

			Vec3 color = (colorMap[i][j] + colorMap[i + 1][j] + colorMap[i + 1][j + 1]) / 3.0f;

			*pointer0.color = Vec4(color, 1.0f);
			*pointer1.color = Vec4(color, 1.0f);
			*pointer2.color = Vec4(color, 1.0f);

			pointer0 = terrainDrawable->getVertexPointerAt(k++);
			pointer1 = terrainDrawable->getVertexPointerAt(k++);
			pointer2 = terrainDrawable->getVertexPointerAt(k++);


			*pointer0.position = toWorldGamePos(i+1, j+1);
			*pointer1.position = toWorldGamePos(i, j+1);
			*pointer2.position = toWorldGamePos(i, j);

			*pointer0.normal = *pointer1.normal = *pointer2.normal = Vec3::cross(*pointer1.position - *pointer0.position, *pointer2.position - *pointer0.position);

			color = (colorMap[i][j] + colorMap[i][j + 1] + colorMap[i + 1][j + 1]) / 3.0f;

			*pointer0.color = Vec4(color, 1.0f);
			*pointer1.color = Vec4(color, 1.0f);
			*pointer2.color = Vec4(color, 1.0f);

		}
	}
	terrainDrawable->build();
}

void WorldMap::render() {

	float oldFar, oldNear;
	if (isScaled) {
		oldFar = gears.game->activeCamera->farPlane;
		oldNear = gears.game->activeCamera->nearPlane;
		gears.game->activeCamera->farPlane = 1000000.0f;
		gears.game->activeCamera->nearPlane = 100.0f;
		gears.game->updateProjectionMatrix();
	}


	shader->setUniform("uDiscardArea", Vec3::zero());
	terrainDrawable->render();
	waterDrawable->render();

	if (isScaled) {
		gears.game->activeCamera->farPlane = oldFar;
		gears.game->activeCamera->nearPlane = oldNear;
		gears.game->updateProjectionMatrix();
	}

	glClear(GL_DEPTH_BUFFER_BIT);
}

Vec3 WorldMap::toGamePos(WorldCoor &coor) {
	return toGamePos(coor, mainCacher);
}

Vec3 WorldMap::toWorldGamePos(int i, int j) {
	return Vec3((float)i, (float)j, heightMap[i][j] * 3.0f / nodeSize);
}

Vec3 WorldMap::toGamePos(WorldCoor &coor, HeightCacher& cacher) const {
	coor.fix(nodeSize);

	if (coor.h == INVALID_HEIGHT) {
		coor.h = getHeightAt(coor, cacher);
	}

	IntVec2 shift = coor.index - anchorPos;

	return Vec3(coor.pos.x + shift.x * nodeSize, coor.pos.y + shift.y * nodeSize, coor.h);
}


float WorldMap::getTreeProbabilityAt(WorldCoor &coor, HeightCacher& cacher) const {
	coor.fix(nodeSize);

	if (coor.h == INVALID_HEIGHT) {
		coor.h = getHeightAt(coor, cacher);
	}

	if (coor.h < 1.0f) {
		return 0.0f;
	}

	return earthMap.getHeightAt(coor);
}

void WorldMap::buildDrainage() {
	drainageNodes.clear();
	for (int i = 0; i < drainageEdgeCount; i++) {
		for (int j = 0; j < drainageEdgeCount; j++) {
			DrainageNode& n = drainageGrid[i][j];
			n.drainageIndex = IntVec2(i, j);
			n.coor.index = IntVec2((int)(i * DRAINAGE_NODE_SIZE / nodeSize), (int)(j * DRAINAGE_NODE_SIZE / nodeSize));
			n.coor.pos = Vec2(i * DRAINAGE_NODE_SIZE - n.coor.index.x * nodeSize, j * DRAINAGE_NODE_SIZE - n.coor.index.y * nodeSize);
			getHeightAt(n.coor);
			n.drainage = 100.0f;

			bool add = true;
			if (i > 0 && i < drainageEdgeCount - 1 && j > 0 && j < drainageEdgeCount - 1) {
				float dt = DRAINAGE_NODE_SIZE;
				float sx = drainageGrid[i + 1][j].coor.h - drainageGrid[i - 1][j].coor.h;
				float sy = drainageGrid[i][j + 1].coor.h - drainageGrid[i][j - 1].coor.h;

				n.normal = Vec3(-sx, sy, 2.0f*dt).normalized();
			} else {
				add = false;
				n.normal = Vec3(0.0f, 0.0f, 1.0f);
			}

			add &= n.coor.h > 0.0f;

			if (add) {
				drainageNodes.push_back(&n);
			}
		}
	}

	IntVec2 dirs[4];
	dirs[0] = IntVec2(-1, 0);
	dirs[1] = IntVec2(1, 0);
	dirs[2] = IntVec2(0, -1);
	dirs[3] = IntVec2(0, 1);

	for (unsigned a = 0; a < drainageNodes.size(); a++) {
		DrainageNode* n = drainageNodes[a];


		float over = n->drainage;

		float t = 0;

		for (int k = 0; k<4; k++) {
			if (n->coor.h > drainageGrid[n->drainageIndex + dirs[k]].coor.h) t += n->coor.h - drainageGrid[n->drainageIndex + dirs[k]].coor.h;
		}

		if (t == 0.0f) continue;

		for (int k = 0; k<4; k++) {
			if (n->coor.h > drainageGrid[n->drainageIndex + dirs[k]].coor.h) {
				drainageGrid[n->drainageIndex + dirs[k]].drainage += over*(n->coor.h - drainageGrid[n->drainageIndex + dirs[k]].coor.h) / t;
			}
		}

	}

	maxDrainage = -100000000.0f;
	minDrainage = 100000000.0f;
	for (int i = 0; i < drainageEdgeCount; i++) {
		for (int j = 0; j < drainageEdgeCount; j++) {
			if (drainageGrid[i][j].coor.h <= 0.0f) {
				drainageGrid[i][j].drainage = 0.0f;
				continue;
			}
			maxDrainage = gmax(maxDrainage, drainageGrid[i][j].drainage);
			minDrainage = gmin(minDrainage, drainageGrid[i][j].drainage);
		}
	}
}

void WorldMap::buildRivers() {
	std::vector<DrainageNode*> nodes;

	IntVec2 dirs[4];
	dirs[0] = IntVec2(-1, 0);
	dirs[1] = IntVec2(1, 0);
	dirs[2] = IntVec2(0, -1);
	dirs[3] = IntVec2(0, 1);

	for (unsigned a = 0; a < drainageNodes.size(); a++) {
		DrainageNode* n = drainageNodes[a];

		bool hasWater = false;
		for (int a = 0; a < 4; a++) {
			if (drainageGrid[dirs[a] + n->drainageIndex].coor.h <= 0.0f) {
				hasWater = true;
			}
		}
		if (hasWater) {
			nodes.push_back(n);
		}

	}

	std::sort(nodes.begin(), nodes.end(), [](DrainageNode* lhs, DrainageNode* rhs) {
		return lhs->drainage > rhs->drainage;
	});


	for (int i = 0; i < drainageEdgeCount; i++) {
		for (int j = 0; j < drainageEdgeCount; j++) {
			DrainageNode& n = drainageGrid[i][j];
			Vec3 p = Vec3(n.drainageIndex.x * DRAINAGE_NODE_SIZE / nodeSize, n.drainageIndex.y * DRAINAGE_NODE_SIZE / nodeSize, heightMap[n.coor.index] * 3.0f / nodeSize);

			//debugRenderer.addLine(p, p + Vec3(0.0f, 0.0f, (n.drainage - minDrainage) * 10.0f / (maxDrainage - minDrainage)), 0xFFFF0000, 100.0f);

		}
	}
 	generateRiver(nodes[0]->drainageIndex, nodes[1]->drainageIndex);
}


Vec3 WorldMap::drainageNodeToVec3(const DrainageNode& node){
	return Vec3(node.drainageIndex.x * DRAINAGE_NODE_SIZE / nodeSize, node.drainageIndex.y * DRAINAGE_NODE_SIZE / nodeSize,
		node.coor.h * 3.0f / nodeSize);
}

bool WorldMap::generateRiver(const IntVec2& startIndex, const IntVec2& endIndex) {
	class SearchNode{
	public:
		IntVec2 index;
		float waterAmount;
		SearchNode(IntVec2 index, float waterAmount){
			this->index = index;
			this->waterAmount = waterAmount;
		}
	};


	class Compare {
	public:
		bool operator() (const SearchNode& node1, const SearchNode& node2) {
			return node1.waterAmount < node2.waterAmount;
		}
	};

	std::priority_queue<SearchNode, std::vector<SearchNode>, Compare> searchQueue;

	debugRenderer.addSphere(drainageNodeToVec3(drainageGrid[startIndex]), 0.5f, 0xFF00FF00, 500.0f);
	debugRenderer.addSphere(drainageNodeToVec3(drainageGrid[endIndex]), 0.5f, 0xFF00FF00, 500.0f);


	/*
	searchQueue.push(SearchNode(startIndex, drainageGrid[startIndex].drainage));

	Grid<bool> added;
	added.init(drainageEdgeCount);
	added.setZero();

	for(int i=0; i<drainageEdgeCount; i++){
		added[0][i] = true;
		added[i][0] = true;
		added[drainageEdgeCount-1][i] = true;
		added[i][drainageEdgeCount-1] = true;
	}
	std::vector<SearchNode> addedNodes;
	while(searchQueue.size()){
		SearchNode n = searchQueue.top();
		searchQueue.pop();
		added[n.index] = true;
		addedNodes.push_back(n);

		IntVec2 maxSide = -1;
		float maxVal = 100.0f;
		for(int i=0; i<IntVec2::sideCount(); i++){
			IntVec2 side = n.index.getSide(i);
			if(added[side] == false && drainageGrid[side].coor.h > 0.0f){

				float goodness1 = drainageGrid[side].drainage;
				float goodness2 = drainageGrid[side].coor.h - drainageGrid[n.index].coor.h;
				float goodness3 = 1000.0f - drainageGrid[side].coor.h;
				float goodness = goodness1 + goodness2 * 0.1f + goodness3 * 0.1f;
				if(goodness > maxVal){
					maxVal = goodness;
					maxSide = side;
				}
			}
		}

		if(maxSide.x != -1){
			float newWaterAmount = n.waterAmount - (drainageGrid[maxSide].coor.h - drainageGrid[n.index].coor.h) * 0.01f;

			searchQueue.push(SearchNode(maxSide, newWaterAmount));
		}
	}



	for (unsigned a = 1; a < addedNodes.size(); a++) {
		
		debugRenderer.addLine(drainageNodeToVec3(drainageGrid[addedNodes[a]]), drainageNodeToVec3(drainageGrid[addedNodes[a-1]]), 0xFF0000FF, 500.0f);

	}*/
	return false;
}
