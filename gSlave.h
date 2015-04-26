#pragma once

#ifndef G_SLAVE_H__
#define G_SLAVE_H__

#include "gTools.h"
#include "gMutex.h"
#include <queue>

class gSlave;
class gSlaveController;

class gSlaveWork {
	bool done;
public:
	int priority;
	gSlaveWork() {
		priority = 0;
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
class Compare {
public:
	bool operator() (const gSlaveWork* slave0, const gSlaveWork* slave1) {
		return slave0->priority < slave1->priority;
	}
};

struct ThreadData {
	gSlave* slave;
	int slaveIndex;
	bool freed;
	gSlaveWork* workToDo;
	gSlaveController* controller;
};

class gSlave {
	ThreadData* sharedData;
	void startSlaveThreadNative();
	gSlaveController* controller;
public:
	gSlave() {
		sharedData = nullptr;
		this->controller = nullptr;
	}

	void startSlaveThread(gSlaveController* controller);

	void freeSlaveThread() {
		assert(sharedData);
		sharedData->freed = true;
		sharedData = nullptr;
	}

	bool isWorking() {
		return sharedData && sharedData->workToDo != nullptr;
	}

	void slaveWorkDone(gSlaveWork* work);

	gSlaveWork* getNextJobForSlave();
};

class gSlaveController {
	int slaveCount;
	gSlave* slaves;
	std::priority_queue<gSlaveWork*, std::vector<gSlaveWork*>, Compare> workForSlavesQueue;
	std::queue<gSlaveWork*> workForMainQueue;
	gMutex controllerMutex;
	friend class gSlave;

	gSlaveWork* getNextJobForSlave() {
		if (workForSlavesQueue.size()) {
			gSlaveWork* work = nullptr;
			controllerMutex.waitAndGetMutex();
			if (workForSlavesQueue.size()) {
				work = workForSlavesQueue.top();
				workForSlavesQueue.pop();
			}
			controllerMutex.releaseMutex();
			return work;
		}
		return nullptr;
	}


	void addMainWork(gSlaveWork* work) {
		controllerMutex.waitAndGetMutex();
		workForMainQueue.push(work);
		controllerMutex.releaseMutex();
	}
public:

	void startSlaves(int slaveCount);
	void freeSlaves();
	void addWork(gSlaveWork* work);
	void update();


	bool hasJobForSlaves() {
		return workForSlavesQueue.size() > 0;
	}
};




#endif 