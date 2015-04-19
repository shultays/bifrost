#include "gDebugRenderer.h"
#include "gGlobals.h"

int lifeTimeToPriority(float lifeTime) {
	if (lifeTime < 0.0f) return 0x0FFFFFFF;
	else return (int)(lifeTime);
}

gDebugRenderer::gDebugRenderer() : gIndexBufferedLineDrawable(VERTEX_PROP_POSITION | VERTEX_PROP_COLOR | 0x4, MAX_LINE_COUNT, false, false) {}

void gDebugRenderer::init() {
	shader = resources.getShader("debug.vs", "debug.ps");

	int stacks = 5;
	int slices = 10;

	for (int p = 0; p < slices; p++) {
		float phi1 = (((float)p) / slices) * 2 * pi;
		float phi2 = (((float)p + 1) / slices) * 2 * pi;

		float sp1 = sin(phi1);
		float sp2 = sin(phi2);
		float cp1 = cos(phi1);
		float cp2 = cos(phi2);

		for (int t = 0; t < stacks; t++) {
			float theta1 = (((float)t) / stacks)*pi;
			float theta2 = (((float)t + 1) / stacks)*pi;

			float ct1 = cos(theta1);
			float st1 = sin(theta1);

			float ct2 = cos(theta2);
			float st2 = sin(theta2);
			Vec3 v0(sp1*st1, cp1*st1, ct1);
			Vec3 v1(sp2*st1, cp2*st1, ct1);
			Vec3 v2(sp1*st2, cp1*st2, ct2);

			sphere.push_back(v0);
			sphere.push_back(v1);
			sphere.push_back(v2);
		}
	}

	build();
}

void gDebugRenderer::addSphere(const Vec3& mid, float radius, unsigned lineColor, float lifeTime) {

	for (unsigned i = 0; i < sphere.size(); i += 3) {
		if (lineCount >= maxLine) {
			break;
		}

		addLine(sphere[i] * radius + mid, sphere[i + 1] * radius + mid, lineColor, lifeTime);
		addLine(sphere[i] * radius + mid, sphere[i + 2] * radius + mid, lineColor, lifeTime);

	}

	build();
}

void gDebugRenderer::addLine(const Vec3& lineBegin, const Vec3& lineEnd, unsigned lineColor, float lifeTime) {
	if (lineCount < maxLine) {
		float timeToDie = lifeTime >= 0.0f ? time.getTime() + lifeTime : lifeTime;

		gLinePointer linePointer = gIndexBufferedLineDrawable::addLine();
		*linePointer.p0.position = lineBegin;
		*linePointer.p0.color = Vec4::fromColor(lineColor);
		*linePointer.p1.position = lineEnd;
		*linePointer.p1.color = Vec4::fromColor(lineColor);
		*(float*)linePointer.p0.extra = timeToDie;
		*(float*)linePointer.p0.extra = timeToDie;

		build();
	}
}

void gDebugRenderer::render() {
	bool hasChange = false;
	for (int i = 0; i < lineCount; i++) {
		gLinePointer linePointer = getLine(i);
		if (*(float*)linePointer.p0.extra >= 0 && *(float*)linePointer.p0.extra < time.getTime()) {
			removeLine(i);
			i--;
			hasChange = true;
			continue;
		}
	}

	if (hasChange) {
		build();
	}

	gIndexBufferedLineDrawable::render();
}

void gDebugRenderer::clear() {
	lineCount = indexCount = vertexCount = 0;
}

