#include "PngExporter.h"


#include <stdio.h>
#include <Math.h>

#include "lodepng.h"

int PngExporter::writeGridToPng(const char* fileName, Grid<Vec3>& grid, int exportType) {
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;
	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;
	for (int i = 0; i < grid.w*grid.h; i++) {
		const Vec3& v = grid.data[i];
		minX = fminf(minX, v.x);
		minY = fminf(minY, v.y);
		minZ = fminf(minZ, v.z);
		maxX = fmaxf(maxX, v.x);
		maxY = fmaxf(maxY, v.y);
		maxZ = fmaxf(maxZ, v.z);
	}
	int t = 0;

	byte* image = new byte[grid.w*grid.h * 4];
	byte* c = image;
	for (int i = 0; i < grid.w; i++) {
		for (int j = 0; j < grid.h; j++) {
			byte r, g, b, a;

			a = 0xFF;
			switch (exportType) {
				case ExportTypeVec3XOnly:
					r = g = b = (byte)(255 * ((grid[i][j].x - minX) / (maxX - minX)));
					break;
				case ExportTypeVec3YOnly:
					r = g = b = (byte)(255 * ((grid[i][j].y - minY) / (maxY - minY)));
					break;
				case ExportTypeVec3ZOnly:
					r = g = b = (byte)(255 * ((grid[i][j].z - minZ) / (maxZ - minZ)));
					break;
				default:
					r = (byte)(255 * ((grid[i][j].x - minX) / (maxX - minX)));
					g = (byte)(255 * ((grid[i][j].y - minY) / (maxY - minY)));
					b = (byte)(255 * ((grid[i][j].z - minZ) / (maxZ - minZ)));
					break;
			}

			*c++ = r;
			*c++ = g;
			*c++ = b;
			*c++ = a;
		}
	}
	unsigned error = lodepng_encode32_file(fileName, image, grid.w, grid.h);

	delete image;
	/*if there's an error, display it*/
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	return error;
}


int PngExporter::writeGridToPng(const char* fileName, Grid<float>& grid, int exportType) {
	float min = FLT_MAX;
	float max = FLT_MIN;
	for (int i = 0; i < grid.w*grid.h; i++) {
		float v = grid.data[i];
		min = fminf(min, v);
		max = fmaxf(max, v);
	}
	int t = 0;

	byte* image = new byte[grid.w*grid.h * 4];
	byte* c = image;
	for (int i = 0; i < grid.w; i++) {
		for (int j = 0; j < grid.h; j++) {
			byte r, g, b, a;

			r = g = b = (byte)(255 * ((grid[i][j] - min) / (max - min)));
			a = 0xFF;


			*c++ = r;
			*c++ = g;
			*c++ = b;
			*c++ = a;
		}
	}
	unsigned error = lodepng_encode32_file(fileName, image, grid.w, grid.h);

	delete image;
	/*if there's an error, display it*/
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	return error;
}