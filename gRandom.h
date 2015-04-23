#pragma once

#ifndef G_RANDOM_H__
#define G_RANDOM_H__

#include "tinymt32.h"
#include <vector>

class gRandom {

	tinymt32_t tinymt;
public:
	gRandom() : gRandom(0x42042) {

	}

	gRandom(std::vector<unsigned int>& seeds) {
		setSeed(seeds);
	}


	gRandom(unsigned int seed) {
		setSeed(seed);
	}

	gRandom(unsigned int seed1, unsigned int seed2) {
		setSeed(seed1, seed2);
	}

	void setSeed(unsigned int seed) {
		tinymt32_init(&tinymt, seed);
	}

	void setSeed(unsigned int seed1, unsigned int seed2) {
		unsigned int seeds[2] = { seed1, seed2 };
		tinymt32_init_by_array(&tinymt, seeds, 2);
	}

	void setSeed(std::vector<unsigned int>& seeds) {
		tinymt32_init_by_array(&tinymt, &seeds[0], seeds.size());
	}

	bool randBool() {
		return (randInt() & 1) == 0;
	}
	bool randBool(float chanceToTrue) {
		return randFloat() < chanceToTrue;
	}
	int randInt() {
		return tinymt32_generate_uint32(&tinymt);
	}

	int randInt(int n) {
		return randInt() % n;
	}

	int randInt(int a, int b) {
		return randInt() % (b - a + 1) + a;
	}

	float randFloat() {
		return tinymt32_generate_float(&tinymt);
	}

	float randFloat(float n) {
		return randFloat() * n;
	}

	float randFloat(float a, float b) {
		return randFloat() * (b - a) + a;
	}
};

#endif