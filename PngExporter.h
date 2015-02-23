#pragma once
#ifndef PNG_EXPORTER_H__

#include "gVec.h"
#include "Grid.h"

enum {
	ExportTypeVec3ZOnly,
	ExportTypeVec3XOnly,
	ExportTypeVec3YOnly,
	ExportTypeVec3AsNormal,
};

class PngExporter {
public:
	static int writeGridToPng(const char* fileName, Grid<Vec3>& grid, int exportType);
	static int writeGridToPng(const char* fileName, Grid<float>& grid, const Vec3& minColor = Vec3(0.0f), const Vec3& maxColor = Vec3(1.0f));

};

#endif