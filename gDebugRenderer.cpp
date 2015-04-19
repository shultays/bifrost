#include "gDebugRenderer.h"
#include "gGlobals.h"

int lifeTimeToPriority(float lifeTime) {
	if (lifeTime < 0.0f) return 0x0FFFFFFF;
	else return (int)(lifeTime);
}

gDebugRenderer::gDebugRenderer() : gIndexBufferedLineDrawable(VERTEX_PROP_POSITION | VERTEX_PROP_COLOR | 0x4, MAX_LINE_COUNT, false, false) {}

void gDebugRenderer::init() {
	shader = resources.getShader("debug.vs", "debug.ps");
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

