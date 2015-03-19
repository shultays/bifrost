#ifndef G_GAME_H__
#define G_GAME_H__

#include "FixedSizedPriorityArray.h"
#include "FixedSizedArray.h"
#include "gCamera.h"
#include "gShader.h"
#include "gMat.h"

//Tickable objects will be ticked before each render
class gTickable {
public:
	int enabled;
	gTickable(bool autoAdd = true);
	virtual ~gTickable();
	virtual void tick(float dt) = 0;
};

//Tickable objects will be updated in a fixed_dt
class gUpdatable {
public:
	int enabled;
	gUpdatable(bool autoAdd = true, int priority = max_priority);
	virtual ~gUpdatable();
	virtual void update(float fixed_dt) = 0;
};


class gRenderable {
	friend class gGame;
private:
	void gRender();

public:
	Mat4 frame;
	int enabled;
	gRenderable(bool autoAdd = true, int priority = max_priority);
	virtual ~gRenderable();
	virtual void render() = 0;
};


class gGame {
	friend class gGears;
	FixedSizedArray<gTickable*, 2048> tickList;
	FixedSizedPriorityArray<gUpdatable*, 2048> updateList;
	FixedSizedPriorityArray<gRenderable*, 2048> renderList;

	FixedSizedArray<Mat4, 32> worldMatrixStack;
	bool worldMatDirty;

	int currentStack;

	void render();

	void tick(float dt) {
		for (int i = tickList.size() - 1; i >= 0; i--) {
			if (tickList[i]->enabled)  tickList[i]->tick(dt);
		}
	}

	void update(float fixed_dt) {
		for (int i = updateList.size() - 1; i >= 0; i--) {
			if (updateList[i]->enabled) updateList[i]->update(fixed_dt);
		}
	}

public:
	gCamera *activeCamera;
	gShader shader;
	gGame(bool setTop = true);

	virtual void init() = 0;
	virtual void resize(int width, int height) = 0;

	void addTickable(gTickable *tickable) {
		tickList.insert(tickable);
	}
	void removeTickable(gTickable *tickable) {
		tickList.remove(tickable);
	}

	void addUpdatable(gUpdatable *updatable, int priority) {
		updateList.insert(updatable, priority);
	}
	void removeUpdatable(gUpdatable *updatable) {
		updateList.remove(updatable);
	}

	void addRenderable(gRenderable *renderable, int priority) {
		renderList.insert(renderable, priority);
	}
	void removeRenderable(gRenderable *renderable) {
		renderList.remove(renderable);
	}

	void resetWorldStack() {
		worldMatDirty = true;
		currentStack = 0;
		worldMatrixStack[currentStack].makeIdentity();
	}

	void pushMatrix() {
		currentStack++;
		worldMatrixStack[currentStack] = worldMatrixStack[currentStack - 1];
	}

	void popMatrix() {
		if (currentStack > 0)
			currentStack--;
	}

	void translate(const Vec3& pos) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].translateBy(pos);
	}
	void translate(float x, float y, float z) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].translateBy(Vec3(x, y, z));
	}
	void rotate(float angle, const Vec3& dir) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].rotateBy(angle, dir);
	}
	void updateShaderUniforms() {
		if (worldMatDirty) {
			shader.setWorldMatrix(worldMatrixStack[currentStack]);
		}
	}
	void rotateX(float angle) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].rotateByX(angle);
	}
	void rotateY(float angle) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].rotateByY(angle);
	}
	void rotateZ(float angle) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].rotateByZ(angle);
	}
	void scale(float s) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].scaleBy(Vec3(s));
	}
	void scale(float x, float y, float z) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].scaleBy(Vec3(x, y, z));
	}
	void scale(const Vec3& s) {
		worldMatDirty = true;
		worldMatrixStack[currentStack].scaleBy(s);
	}
	void multiply(const Mat4& m) {
		worldMatDirty = true;
		worldMatrixStack[currentStack] *= m;
	}
};


#endif