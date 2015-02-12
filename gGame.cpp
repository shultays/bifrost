#include "gGame.h"
#include "gGlobals.h"

gTickable::gTickable(bool autoAdd){
	enabled = true;
	if (autoAdd){
		gears.game->addTickable(this);
	}
}

gTickable::~gTickable(){
	enabled = true;
	gears.game->removeTickable(this);
}


gUpdatable::gUpdatable(bool autoAdd, int priority){
	enabled = true;
	if (autoAdd){
		gears.game->addUpdatable(this, priority);
	}
}

gUpdatable::~gUpdatable(){
	enabled = true;
	gears.game->removeUpdatable(this);
}

gRenderable::gRenderable(bool autoAdd, int priority){
	enabled = true;
	if (autoAdd){
		gears.game->addRenderable(this, priority);
	}
}

gRenderable::~gRenderable(){
	enabled = true;
	gears.game->removeRenderable(this);
}

gGame::gGame(bool setTop){
	if (setTop){
		gears.game = this;
	}
}