#pragma once

#ifndef G_RENDERABLE_GROUP_H__
#define G_RENDERABLE_GROUP_H__

#include "gGame.h"
#include <vector>

class gRenderableGroup : public gRenderable {
	std::vector<gRenderable*> children;

public:
	gRenderableGroup(bool autoAdd = true, int priority = max_priority) : gRenderable(autoAdd, priority) {

	}



	int getChildCount() const {
		return children.size();
	}

	gRenderable* getChildrenAt(int index) const {
		return children.at(index);
	}

	void addRenderable(gRenderable* renderable) {
		children.push_back(renderable);
	}

	void render() override {
		for (unsigned i = 0; i < children.size(); i++) {
			children.at(i)->gRender();
		}
	}
};

#endif