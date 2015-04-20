#pragma once
#ifndef TREE_GENERATOR_H__
#define TREE_GENERATOR_H__

#include "gTools.h"

class gRenderable;

class TreeGenerator {

public:

	static gRenderable* generateTree(Vec3 pos);
};


#endif