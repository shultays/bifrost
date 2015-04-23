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

	static void generateTree(Vec3 pos, std::vector<TreeTriangle>& triangles, gRandom& random);
};


#endif