#ifndef VALHALLA_H__
#define VALHALLA_H__

#include "gGame.h"
#include "Tools.h"

class WorldMap;
class gFocusCamera;

class Valhalla : public gGame, public gTickable, public gUpdatable {
	WorldMap *world;
	gFocusCamera* camera;
	gFPSCamera* fpsCamera;

	WorldCoor playerCoor;

	bool isFPS;

	void init() override;
	void tick(float dt) override;
	void update(float fixed_dt) override;
	void resize(int width, int height) override;
};

#endif