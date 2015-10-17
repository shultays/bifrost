#include "PerlinShell.h"
#include "WorldMap.h"

#include "gVec.h"
#include "Tools.h"
#include "WorldMap.h"
#include "gGlobals.h"

#include <functional>

float PerlinShell::getNodeHeight(int i, int j) const {
	return (((unsigned int)(hasher(i + edgeCount*j)) ^ seed) > acceptHash) ? 1.0f : 0.0f;
}

PerlinShell::PerlinShell(float mapSize, float nodeSize, int edgeCount, float minHeight, float maxHeight, float percentage, float pow_val, unsigned int seed) {
	maxHash = std::numeric_limits<size_t>::max();
	inited = true;
	this->mapSize = mapSize;
	this->nodeSize = nodeSize;

	this->cellSize = mapSize / (edgeCount - 1);
	this->cellPerNode = nodeSize / cellSize;

	if (seed == -1) {
		seed = random.randInt();
	}
	this->seed = seed;
	this->edgeCount = edgeCount;
	this->maxHeight = maxHeight;
	this->minHeight = minHeight;
	this->diffHeight = maxHeight - minHeight;
	this->pow_val = pow_val;
	if (almost_equal(pow_val, 1.0f)) {
		pow_type = pow_1;
	} else if (almost_equal(pow_val, 2.0f)) {
		pow_type = pow_2;
	} else if (almost_equal(pow_val, 0.5f)) {
		pow_type = pow_0_5;
	} else if (almost_equal(pow_val, round(pow_val))) {
		pow_type = (int)round(pow_val);
	} else {
		pow_type = pow_n;
	}
	acceptHash = (unsigned int)(maxHash * percentage);

	unsigned int t = seed;
	float rotation = (t & 0xFF) / 255.0f;
	t /= 0xFF + 1;

	rotate.makeIdentity();
	rotate.rotateBy(pi_2*rotation);

	float addX = (t & 0xFFF) / (0xFFF + 1.0f);
	t /= 0xFFF + 1;
	float addY = (t & 0xFFF) / (0xFFF + 1.0f);
	t /= 0xFFF + 1;

	add = Vec2(addX, addY);
}
float PerlinShell::getHeightAt(const WorldCoor& coor) const {
	assert(inited);
	Vec2 p;
	p.x = (float)coor.index.x;
	p.y = (float)coor.index.y;

	p = p*rotate;
	p += add;

	IntVec2 pi;
	pi.x = (int)p.x;
	pi.y = (int)p.y;
	p.x -= pi.x;
	p.y -= pi.y;

	Vec2 p2 = coor.pos;
	p2 = p2*rotate;

	p = p * nodeSize + p2;


	int i, j;
	float dx = pi.x * cellPerNode;
	float dy = pi.y * cellPerNode;
	i = (int)floor(dx);
	j = (int)floor(dy);
	dx -= i;
	dy -= j;
	dx += p.x / cellSize;
	dy += p.y / cellSize;
	int ti = (int)floor(dx);
	int tj = (int)floor(dy);
	i += ti;
	j += tj;
	dx -= ti;
	dy -= tj;


	// p2 -- p4
	// |  \  |
	// p0 -- p1

	float r;

	if (dx + dy < 1.0f) {
		float p0 = getNodeHeight(i, j);
		float p1 = getNodeHeight(i + 1, j);
		float p2 = getNodeHeight(i, j + 1);


		r = p0 * (1.0f - dx - dy) + dx * p1 + dy * p2;
	} else {
		float p4 = getNodeHeight(i + 1, j + 1);
		float p1 = getNodeHeight(i + 1, j);
		float p2 = getNodeHeight(i, j + 1);

		dx = 1.0f - dx;
		dy = 1.0f - dy;

		r = p4 * (1.0f - dx - dy) + dx * p2 + dy * p1;
	}

	switch (pow_type) {
		case pow_1:
			return r * diffHeight + minHeight;
		case pow_2:
			return r*r * diffHeight + minHeight;
		case pow_0_5:
			return sqrt(r) * diffHeight + minHeight;
		case pow_n:
			return pow(r, pow_val) * diffHeight + minHeight;
		default:
		{
				   float r2 = r;
				   for (int i = 1; i < pow_type; i++) r2 *= r;
				   return r2 * diffHeight + minHeight;
		}
	}

	return 0.0f;
}
