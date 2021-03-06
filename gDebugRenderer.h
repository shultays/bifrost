#pragma once

#ifndef G_DEBUG_RENDERER_H__
#define G_DEBUG_RENDERER_H__

#include "gIndexBufferedDrawable.h"

#define LIFE_TIME_INFINITE -1.0f
#define LIFE_TIME_ONE_FRAME 0.0f

#define MAX_LINE_COUNT 150000

class gDebugRenderer : protected gIndexBufferedLineDrawable {
	friend class gGame;
	/*
	class LineData {
	public:
	int index;
	float timeToDie;
	LineData() {}
	LineData(int index, float timeToDie) {
	this->index = index;
	this->timeToDie = timeToDie;
	}
	};

	FixedSizedPriorityArray<LineData, MAX_LINE_COUNT> lines;
	*/

	std::vector<Vec3> sphere;
	bool needsBuild;
public:

	gDebugRenderer();
	void init();
	void addSphere(const Vec3& mid, float radius, unsigned lineColor = 0xFFFFFFFF, float lifeTime = LIFE_TIME_ONE_FRAME);
	void addLine(const Vec3& lineBegin, const Vec3& lineEnd, unsigned lineColor = 0xFFFFFFFF, float lifeTime = LIFE_TIME_ONE_FRAME);

	void render() override;

	void clear();
};


#endif