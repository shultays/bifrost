#pragma once

#ifndef G_SLAVE_H__
#define G_SLAVE_H__

#include "gTools.h"
#include <queue>

class gSlave;

class gSlaveWork {
	bool done;
public:
	gSlaveWork() {
		done = false;
	}
	void setIsDone(bool done) {
		this->done = done;
	}
	bool isDone() {
		return done;
	}
	virtual void runOnSlave() = 0;
	virtual void runOnMain() = 0;
};


struct ThreadData {
	gSlave* gSlave;
	int slaveIndex;
	bool freed;
	gSlaveWork* workToDo;
	bool slaveWorkDone;
};

class gSlave {
	ThreadData* sharedData;
	void startSlaveThreadNative();

public:
	gSlave() {
		sharedData = nullptr;
	}

	void startSlaveThread();

	void freeSlaveThread() {
		assert(sharedData);
		sharedData->freed = true;
		sharedData = nullptr;
	}

	bool isWorking() {
		return sharedData && sharedData->workToDo != nullptr;
	}

	void update() {
		if (sharedData) {
			if (sharedData->workToDo && sharedData->slaveWorkDone) {
				sharedData->workToDo->runOnMain();
				sharedData->workToDo->setIsDone(true);
				sharedData->workToDo = nullptr;
				sharedData->slaveWorkDone = false;
			}
		}
	}

	void startWork(gSlaveWork* work) {
		assert(!isWorking());
		sharedData->slaveWorkDone = false;
		sharedData->workToDo = work;
	}

};

class gSlaveController {
	int slaveCount;
	gSlave* slaves;
	std::queue<gSlaveWork*> workQueue;
public:

	void startSlaves(int slaveCount);
	void freeSlaves();
	void addWork(gSlaveWork* work);
	void update();
};




#endif 