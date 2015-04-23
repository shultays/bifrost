#include "Perlin.h"
#include "gGlobals.h"


void addRandomMapC(int w, int h, float k, int size, Grid<float> &map, int chance) {
	Grid<int> temp(w + 2, h + 2);
	int i, j, a, b;
	float x, y;
	int r = (int)((w + 2)*(h + 2)*(chance / 100.0f));
	// fprintf(fout, "%d\n", r);

	for (i = 0; i < w + 2; i++) {
		for (j = 0; j < h + 2; j++) {
			if (r) {
				r--;
				temp[i][j] = 1;
			} else temp[i][j] = 0;
			a = random.randInt(i + 1);

			if (a != i) {
				b = random.randInt(h + 2);
			} else {
				b = random.randInt(j + 1);
			}
			swapt(temp[i][j], temp[a][b]);
		}
	}

	float rx = random.randFloat();
	float ry = random.randFloat();
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {

			a = (int)((i*w / ((float)size)) + rx);
			b = (int)((j*h / ((float)size)) + ry);
			x = (rx + (i*w) / ((float)size)) - a;
			y = (ry + (j*h) / ((float)size)) - b;

			float t1 = temp[a][b] * (1 - x) + temp[a + 1][b] * x;
			float t2 = temp[a][b + 1] * (1 - x) + temp[a + 1][b + 1] * x;

			float t = t1*(1 - y) + t2*y;

			map[i][j] += t*k;
		}
	}
}


void addRandomMap(int w, int h, float k, int size, Grid<float> &map) {
	addRandomMapC(w, h, k, size, map, random.randInt(20, 60));
}

