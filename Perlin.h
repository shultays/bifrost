#ifndef PERLIN_H
#define PERLIN_H

#include "Array2D.h"

#include "gTools.h"


void addRandomMapC(int w, int h, float k, int size, Array2D<float> &map, int chance);

void addRandomMap(int w, int h, float k, int size, Array2D<float> &map);

void lcg_addRandomMap(int x, int y, int s, float k, Array2D<float> &map);

#endif
