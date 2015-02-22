#ifndef PERLIN_H
#define PERLIN_H

#include "Grid.h"

#include "gTools.h"


void addRandomMapC(int w, int h, float k, int size, Grid<float> &map, int chance);

void addRandomMap(int w, int h, float k, int size, Grid<float> &map);

void lcg_addRandomMap(int x, int y, int s, float k, Grid<float> &map);

#endif
