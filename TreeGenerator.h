#pragma once
#ifndef TREE_GENERATOR_H__
#define TREE_GENERATOR_H__

#include "gTools.h"
#include "gRandom.h"
#include <vector>



struct TreeTriangle {
	Vec3 vertices[3];
	Vec3 color;
};

class TreeGenerator {

public:

	static void generateTree(const Vec3& pos, std::vector<TreeTriangle>& triangles, gRandom& random);
	static void generateTree2(const Vec3& pos, std::vector<TreeTriangle>& triangles, gRandom& random);
	static void generateCloud(const Vec3& pos, const Vec3& dir, std::vector<TreeTriangle>& triangles, gRandom& random);
};


#endif