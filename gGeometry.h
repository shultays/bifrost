#pragma once

#ifndef G_GEOMETRY_H__
#define G_GEOMETRY_H__

#include "gVec.h"


class Sphere {
private:
	void build() {
		radiusSquared = radius * radius;
	}
public:
	Vec3 center;
	float radius;
	float radiusSquared;

	Sphere(const Vec3& center, float radius) {
		this->center = center;
		this->radius = radius;
		build();
	}

};

class LineSegment {
	void build() {
		this->distanceSquared = p0.distanceSquared(p1);
		this->distance = sqrtf(distanceSquared);
	}
public:
	Vec3 p0, p1;
	float distance;
	float distanceSquared;

	LineSegment(const Vec3& p0, const Vec3& p1) {
		this->p0 = p0;
		this->p1 = p1;
		build();
	}
};

class Capsule : public LineSegment {
	void build() {
		this->radius0Squared = radius0 * radius0;
		this->radius1Squared = radius1 * radius1;
	}
public:
	float radius0;
	float radius1;
	float radius0Squared;
	float radius1Squared;

	Capsule(const Vec3& p0, const Vec3& p1, float radius) : LineSegment(p0, p1) {
		radius0 = radius;
		radius1 = radius;
		build();
	}
};


class gGeometry {
public:
	static float lineSegmentPointDistanceSquared(const LineSegment& lineSegment, const Vec3& point, Vec3& pointOnLine, float& weightOnLine) {
		if (lineSegment.distanceSquared == 0.0) return Vec3::distanceSquared(lineSegment.p0, point);

		weightOnLine = Vec3::dot(lineSegment.p0 - point, lineSegment.p1 - point) / lineSegment.distanceSquared;

		if (weightOnLine < 0.0) pointOnLine = lineSegment.p0;
		else if (weightOnLine > 1.0) pointOnLine = lineSegment.p1;
		else pointOnLine = lerp(lineSegment.p0, lineSegment.p1, weightOnLine);

		return Vec3::distanceSquared(point, pointOnLine);
	}

	static float lineSegmentPointDistanceSquared(const LineSegment& lineSegment, const Vec3& point) {
		Vec3 pointOnLine;
		float weightOnLine;
		return lineSegmentPointDistanceSquared(lineSegment, point, pointOnLine, weightOnLine);
	}

	static float lineSegmentPointDistance(const LineSegment& lineSegment, const Vec3& point, Vec3& pointOnLine, float weightOnLine) {
		return sqrtf(lineSegmentPointDistanceSquared(lineSegment, point, pointOnLine, weightOnLine));
	}

	static float lineSegmentPointDistance(const LineSegment& lineSegment, const Vec3& point) {
		Vec3 pointOnLine;
		float weightOnLine;
		return lineSegmentPointDistance(lineSegment, point, pointOnLine, weightOnLine);
	}

	static bool contains(const Sphere& big, const Sphere& small) {
		float distanceSquared = Vec3::distanceSquared(big.center, small.center);

		return distanceSquared + small.radiusSquared <= big.radiusSquared;
	}


	static bool contains(const Sphere& big, const Capsule& small) {
		float d0 = small.p0.distanceSquared(big.center);
		float d1 = small.p1.distanceSquared(big.center);


		return d0 + small.radius0Squared <= big.radiusSquared && d1 + small.radius1Squared <= big.radiusSquared;
	}
};

#endif