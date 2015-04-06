#include "gSlave.h"

int slaveIndex = 0;

void gSlave::startSlaveThread() {
	assert(!sharedData);
	ThreadData* data = new ThreadData;
	data->gSlave = this;
	data->slaveIndex = ++slaveIndex;
	data->freed = false;
	data->workToDo = nullptr;
	data->slaveWorkDone = false;
	sharedData = data;

	startSlaveThreadNative();
}



void gSlaveController::startSlaves(int slaveCount) {
	this->slaveCount = slaveCount;
	slaves = new gSlave[slaveCount];
	for (int i = 0; i < slaveCount; i++) {
		slaves[i].startSlaveThread();
	}
}

void gSlaveController::freeSlaves() {
	for (int i = 0; i < slaveCount; i++) {
		slaves[i].freeSlaveThread();
	}
	delete[] slaves;
	slaveCount = 0;
}

void gSlaveController::update() {
	for (int i = 0; i < slaveCount; i++) {
		slaves[i].update();
	}

	while (workQueue.size() > 0) {
		bool hasFree = false;
		for (int i = 0; i < slaveCount; i++) {
			if (slaves[i].isWorking() == false) {
				gSlaveWork* work = workQueue.front();
				workQueue.pop();
				slaves[i].startWork(work);
				hasFree = true;
				break;
			}
		}

		if (hasFree == false) {
			break;
		}
	}
}


void gSlaveController::addWork(gSlaveWork* work) {
	for (int i = 0; i < slaveCount; i++) {
		if (slaves[i].isWorking() == false) {
			slaves[i].startWork(work);
			return;
		}
	}
	workQueue.push(work);
}