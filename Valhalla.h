#ifndef VALHALLA_H__
#define VALHALLA_H__

#include "gGame.h"

class Valhalla : public gGame, public gTickable{

	void init() override;
	void tick(float dt) override;
	void resize(int width, int height) override;
};

#endif