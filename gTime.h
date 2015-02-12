#ifndef G_TIME_H__
#define G_TIME_H__

#include "gTools.h"

class gTime{
	friend class gGears;

	float currentTime;
	float dt;
	float fixed_dt;

public:

	gTime(){
		currentTime = (float)glfwGetTime();
		dt = 0.0f;
	}

	float getTime(){
		return currentTime;
	}
	float getDt(){
		return dt;
	}
	float getFixed_dt(){
		return fixed_dt;
	}
	void tick(){
		float newTime = (float)glfwGetTime();
		dt = newTime - currentTime;
		currentTime = newTime;
	}
};

#endif